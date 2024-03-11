#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Navigation.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "RecastDump.h"

// 추측하기로 대강의 구조?
// rcCongig = 그냥 메쉬 어떻게 빌드 할건지에 대한 세팅

bool Navigation::handleBuild()
{
	const float* bmin = 0;
	const float* bmax = 0;
	const float* verts = 0;
	const int nverts = 0;
	const int* tris = 0;
	const int ntris = 0;

	//
	// 1단계. 빌드 구성을 초기화합니다.
	//

	// GUI에서 빌드 구성 초기화

	/// m_config 의 타입은 rcConfig 인데 rc는 이 api에서 recast의 약자로 줄곳 쓰이므로
	/// 대충 recastConfiguration 같은 의미인것 같음.
	/// rcConfig의 내용은 Recast.h에 정말 잘 나와 있으니 꼭 볼것 
	memset(&_cfg, 0, sizeof(_cfg));	// m_cfg만큼의 메모리를 0으로 초기화
	_cfg.cs = _cellSize;
	_cfg.ch = _cellHeight;
	_cfg.walkableSlopeAngle		= _agentMaxSlope;
	_cfg.walkableHeight			= (int)ceilf(_agentHeight / _cfg.ch);
	_cfg.walkableClimb			= (int)floorf(_agentMaxClimb / _cfg.ch);
	_cfg.walkableRadius			= (int)ceilf(_agentRadius / _cfg.cs);
	_cfg.maxEdgeLen				= (int)(_edgeMaxLen / _cellSize);
	_cfg.maxSimplificationError = _edgeMaxError;
	_cfg.minRegionArea			= (int)rcSqr(_regionMinSize);		// Note: area = size*size
	_cfg.mergeRegionArea		= (int)rcSqr(_regionMergeSize);	// Note: area = size*size
	_cfg.maxVertsPerPoly		= (int)_vertsPerPoly;
	_cfg.detailSampleDist		= _detailSampleDist < 0.9f ? 0 : _cellSize * _detailSampleDist;
	_cfg.detailSampleMaxError	= _cellHeight * _detailSampleMaxError;

	// 내비게이션이 빌드될 영역을 설정합니다.
	// 여기서는 입력 메쉬의 바운드가 사용되지만
	// 영역은 사용자 정의 상자 등으로 지정할 수 있습니다.
	rcVcopy(_cfg.bmin, bmin);					// 드디어 recast.h를 참조하는 함수.
	rcVcopy(_cfg.bmax, bmax);
	rcCalcGridSize(_cfg.bmin, _cfg.bmax, _cfg.cs, &_cfg.width, &_cfg.height);

	// 빌드 시간 수집을 재설정합니다.
	_ctx->resetTimers();

	// 빌드 프로세스를 시작합니다.	
	_ctx->startTimer(RC_TIMER_TOTAL);

	_ctx->log(RC_LOG_PROGRESS, "Building navigation:");
	_ctx->log(RC_LOG_PROGRESS, " - %d x %d cells", _cfg.width, _cfg.height);
	_ctx->log(RC_LOG_PROGRESS, " - %.1fK verts, %.1fK tris", nverts / 1000.0f, ntris / 1000.0f);

	//
	// 2단계. 입력 폴리곤 수프를 래스터화합니다.
	//

	// 입력 데이터를 래스터화할 복셀 하이트필드를 할당합니다.
	_solid = rcAllocHeightfield();
	if (!_solid)
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
		return false;
	}
	if (!rcCreateHeightfield(_ctx, *_solid, _cfg.width, _cfg.height, _cfg.bmin, _cfg.bmax, _cfg.cs, _cfg.ch))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
		return false;
	}

	// 삼각형 영역 유형을 담을 수 있는 배열을 할당합니다.
	// 처리해야 하는 메시가 여러 개 있는 경우, 처리해야 하는 최대 트라이앵글 수를 저장할
	// 처리해야 하는 트라이앵글의 최대 개수를 담을 수 있는 배열을 할당합니다.
	_triareas = new unsigned char[ntris];
	if (!_triareas)
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'm_triareas' (%d).", ntris);
		return false;
	}

	// 기울기에 따라 걸을 수 있는 삼각형을 찾아 래스터화합니다.
	// 입력 데이터가 여러 개의 메쉬인 경우 여기에서 변환하여 각 메쉬의
	// 각 메시의 are 유형을 계산하여 래스터화합니다.
	memset(_triareas, 0, ntris * sizeof(unsigned char));
	rcMarkWalkableTriangles(_ctx, _cfg.walkableSlopeAngle, verts, nverts, tris, ntris, _triareas);
	if (!rcRasterizeTriangles(_ctx, verts, nverts, tris, _triareas, ntris, *_solid, _cfg.walkableClimb))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not rasterize triangles.");
		return false;
	}

	if (!_keepInterResults)
	{
		delete[] _triareas;
		_triareas = 0;
	}

	//
	// 3단계. 걷기 가능한 표면을 필터링합니다.
	//

	// 모든 지오메트리가 래스터화되면 필터링의 초기 패스를 수행합니다.
	// 보수적인 래스터화로 인해 발생하는 불필요한 돌출부를 제거하고
	// 캐릭터가 서 있을 수 없는 스팬을 필터링합니다.
	if (_filterLowHangingObstacles)
		rcFilterLowHangingWalkableObstacles(_ctx, _cfg.walkableClimb, *_solid);
	if (_filterLedgeSpans)
		rcFilterLedgeSpans(_ctx, _cfg.walkableHeight, _cfg.walkableClimb, *_solid);
	if (_filterWalkableLowHeightSpans)
		rcFilterWalkableLowHeightSpans(_ctx, _cfg.walkableHeight, *_solid);


	//
	// 4단계. 보행 가능한 표면을 단순한 영역으로 분할합니다.
	//

	// 이제부터 더 빠르게 처리할 수 있도록 높이 필드를 압축합니다.
	// 이렇게 하면 더 많은 캐시 일관된 데이터뿐만 아니라 걷기 가능한 셀 사이의 이웃이
	// 사이의 일관된 데이터를 더 많이 계산할 수 있습니다.
	_chf = rcAllocCompactHeightfield();
	if (!_chf)
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
		return false;
	}
	if (!rcBuildCompactHeightfield(_ctx, _cfg.walkableHeight, _cfg.walkableClimb, *_solid, *_chf))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
		return false;
	}

	if (!_keepInterResults)
	{
		rcFreeHeightField(_solid);
		_solid = 0;
	}

	// 에이전트 반경에 따라 걷기 가능한 영역을 침식합니다.
	if (!rcErodeWalkableArea(_ctx, _cfg.walkableRadius, *_chf))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
		return false;
	}

//	InputGeom.h가 필요한 부분... 이 API에 포함되는 부류가 아니라서 제외
// 	const ConvexVolume* vols = m_geom->getConvexVolumes();
// 	for (int i = 0; i < m_geom->getConvexVolumeCount(); ++i)
// 		rcMarkConvexPolyArea(m_ctx, vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *m_chf);


	// 나중에 간단한 알고리즘을 사용하여 보행 가능한 영역을 삼각 측량할 수 있도록 높이 필드를 분할합니다.
	// 분할 방법에는 3가지가 있으며, 각각 장단점이 있습니다:
	// 1) 유역 파티셔닝
	// - 고전적인 리캐스트 파티셔닝
	// - 가장 멋진 테셀레이션을 생성합니다.
	// - 보통 가장 느림
	// - 하이트필드를 구멍이나 겹침이 없는 멋진 영역으로 분할합니다.
	// - 이 메서드가 구멍과 겹침을 생성하는 코너 케이스가 몇 가지 있습니다.
	// 작은 장애물이 큰 열린 영역에 가까울 때 구멍이 생길 수 있습니다(삼각 측량으로 처리할 수 있음).
	// - 좁은 나선형 복도(예: 계단)가 있는 경우 겹침이 발생할 수 있으며, 이로 인해 삼각 측량이 실패할 수 있습니다.
	// * 일반적으로 내비메쉬를 미리 계산하는 것이 가장 좋으며, 넓은 개방 영역이 있는 경우 이 방법을 사용합니다.
	// 2) 모노톤 파티셔닝
	// - 가장 빠름
	// - 하이트필드를 구멍과 겹침이 없는 영역으로 분할합니다(보장).
	// - 길고 얇은 다각형을 생성하여 때때로 우회 경로를 생성합니다.
	// * 빠른 내비메시 생성을 원한다면 이 옵션을 사용하세요.
	// 3) 레이어 분할
	// - 매우 빠름
	// - 높이 필드를 겹치지 않는 영역으로 분할합니다.
	// - 구멍에 대처하기 위해 삼각 측량 코드에 의존합니다 (따라서 모노톤 파티셔닝보다 느립니다).
	// - 모노톤 파티셔닝보다 더 나은 트라이앵글을 생성합니다.
	// - 분수령 분할의 코너 케이스가 없습니다.
	// - 느리고 약간 보기 흉한 테셀레이션을 생성할 수 있습니다(모노톤보다는 낫습니다).
	// 작은 장애물이 있는 넓은 열린 공간이 있는 경우(타일을 사용하는 경우 문제 없음)
	// * 중간 및 작은 크기의 타일이 있는 타일형 내비메시에 사용하기 좋은 선택입니다.

	if (_partitionType == SAMPLE_PARTITION_WATERSHED)
	{
		// 걷기 가능한 표면을 따라 거리 필드를 계산하여 영역 분할을 준비합니다.
		if (!rcBuildDistanceField(_ctx, *_chf))
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build distance field.");
			return false;
		}

		// 걷기 좋은 표면을 구멍이 없는 단순한 영역으로 분할합니다.
		if (!rcBuildRegions(_ctx, *_chf, 0, _cfg.minRegionArea, _cfg.mergeRegionArea))
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build watershed regions.");
			return false;
		}
	}
	else if (_partitionType == SAMPLE_PARTITION_MONOTONE)
	{
		// 걷기 좋은 표면을 구멍이 없는 단순한 영역으로 분할합니다.
		// 모노톤 파티셔닝에는 디스턴스필드가 필요하지 않습니다.
		if (!rcBuildRegionsMonotone(_ctx, *_chf, 0, _cfg.minRegionArea, _cfg.mergeRegionArea))
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build monotone regions.");
			return false;
		}
	}
	else // 샘플_파티션_레이어
	{
		// 걷기 좋은 표면을 구멍이 없는 단순한 영역으로 분할합니다.
		if (!rcBuildLayerRegions(_ctx, *_chf, 0, _cfg.minRegionArea))
		{
			_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build layer regions.");
			return false;
		}
	}

	//
	// 5단계. 영역 윤곽을 추적하고 단순화합니다.
	//

	// 윤곽선을 만듭니다.
	_cset = rcAllocContourSet();
	if (!_cset)
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'cset'.");
		return false;
	}
	if (!rcBuildContours(_ctx, *_chf, _cfg.maxSimplificationError, _cfg.maxEdgeLen, *_cset))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create contours.");
		return false;
	}

	//
	// 6단계. 윤곽선에서 다각형 메쉬를 만듭니다.
	//

	// 윤곽선에서 다각형 내비 메쉬를 만듭니다.
	_pmesh = rcAllocPolyMesh();
	if (!_pmesh)
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmesh'.");
		return false;
	}
	if (!rcBuildPolyMesh(_ctx, *_cset, _cfg.maxVertsPerPoly, *_pmesh))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not triangulate contours.");
		return false;
	}

	//
	// 7단계. 각 다각형의 대략적인 높이에 접근할 수 있는 디테일 메쉬를 만듭니다.
	//

	_dmesh = rcAllocPolyMeshDetail();
	if (!_dmesh)
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmdtl'.");
		return false;
	}

	if (!rcBuildPolyMeshDetail(_ctx, *_pmesh, *_chf, _cfg.detailSampleDist, _cfg.detailSampleMaxError, *_dmesh))
	{
		_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build detail mesh.");
		return false;
	}

	if (!_keepInterResults)
	{
		rcFreeCompactHeightfield(_chf);
		_chf = 0;
		rcFreeContourSet(_cset);
		_cset = 0;
	}

	// 이제 내비게이션 메시 데이터가 준비되었으므로 m_pmesh에서 액세스할 수 있습니다.
	// 데이터에 액세스하는 방법은 duDebugDrawPolyMesh 또는 dtCreateNavMeshData 예제를 참조하세요.

	//
	// (선택 사항) 8단계. 리캐스트 폴리 메시에서 디투어 데이터를 생성합니다.
	//

	// GUI는 폴리곤당 최대 포인트를 디투어가 처리할 수 있는 것보다 더 많이 허용할 수 있습니다.
	// 제한을 초과하지 않는 경우에만 우회 내비메시를 빌드합니다.
	if (_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
	{
		unsigned char* navData = 0;
		int navDataSize = 0;

		// 영역에서 폴리 플래그를 업데이트합니다.
		for (int i = 0; i < _pmesh->npolys; ++i)
		{
			if (_pmesh->areas[i] == RC_WALKABLE_AREA)
				_pmesh->areas[i] = SAMPLE_POLYAREA_GROUND;

			if (_pmesh->areas[i] == SAMPLE_POLYAREA_GROUND ||
				_pmesh->areas[i] == SAMPLE_POLYAREA_GRASS	||
				_pmesh->areas[i] == SAMPLE_POLYAREA_ROAD)
			{
				_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
			}
			else if (_pmesh->areas[i] == SAMPLE_POLYAREA_WATER)
			{
				_pmesh->flags[i] = SAMPLE_POLYFLAGS_SWIM;
			}
			else if (_pmesh->areas[i] == SAMPLE_POLYAREA_DOOR)
			{
				_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
			}
		}

		/// 이부분이 지금 최대 고비
		/// InputGeom은 지금 하필 obj를 읽고와서 만들어주는 친구라서 이부분만 잘 해결해 준다면 돌려볼 수 있을것 같은데
		/// 이거 한번 현재형이랑 이야기를 좀 해봐야 할듯
		dtNavMeshCreateParams params;
		memset(&params, 0, sizeof(params));
		params.verts			= _pmesh->verts;
		params.vertCount		= _pmesh->nverts;
		params.polys			= _pmesh->polys;
		params.polyAreas		= _pmesh->areas;
		params.polyFlags		= _pmesh->flags;
		params.polyCount		= _pmesh->npolys;
		params.nvp				= _pmesh->nvp;
		params.detailMeshes		= _dmesh->meshes;
		params.detailVerts		= _dmesh->verts;
		params.detailVertsCount = _dmesh->nverts;
		params.detailTris		= _dmesh->tris;
		params.detailTriCount	= _dmesh->ntris;
		params.offMeshConVerts	= m_geom->getOffMeshConnectionVerts();
		params.offMeshConRad	= m_geom->getOffMeshConnectionRads();
		params.offMeshConDir	= m_geom->getOffMeshConnectionDirs();
		params.offMeshConAreas	= m_geom->getOffMeshConnectionAreas();
		params.offMeshConFlags	= m_geom->getOffMeshConnectionFlags();
		params.offMeshConUserID = m_geom->getOffMeshConnectionId();
		params.offMeshConCount	= m_geom->getOffMeshConnectionCount();
		params.walkableHeight	= _agentHeight;
		params.walkableRadius	= _agentRadius;
		params.walkableClimb	= _agentMaxClimb;
		rcVcopy(params.bmin, _pmesh->bmin);
		rcVcopy(params.bmax, _pmesh->bmax);
		params.cs = _cfg.cs;
		params.ch = _cfg.ch;
		params.buildBvTree = true;

		if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
		{
			_ctx->log(RC_LOG_ERROR, "Could not build Detour navmesh.");
			return false;
		}

		_navMesh = dtAllocNavMesh();
		if (!_navMesh)
		{
			dtFree(navData);
			_ctx->log(RC_LOG_ERROR, "Could not create Detour navmesh");
			return false;
		}

		dtStatus status;

		status = _navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
		if (dtStatusFailed(status))
		{
			dtFree(navData);
			_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh");
			return false;
		}

		status = _navQuery->init(_navMesh, 2048);
		if (dtStatusFailed(status))
		{
			_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh query");
			return false;
		}
	}

	_ctx->stopTimer(RC_TIMER_TOTAL);

	// 성능 통계를 표시합니다.
	duLogBuildTimes(*_ctx, _ctx->getAccumulatedTime(RC_TIMER_TOTAL));
	_ctx->log(RC_LOG_PROGRESS, ">> Polymesh: %d vertices  %d polygons", _pmesh->nverts, _pmesh->npolys);

	_totalBuildTimeMs = _ctx->getAccumulatedTime(RC_TIMER_TOTAL) / 1000.0f;

// 	if (m_tool)
// 		m_tool->init(this);
// 	initToolStates(this);

	return true;
}
