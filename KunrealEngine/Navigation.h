#pragma once

#include "DetourNavMesh.h"
#include "Recast.h"

/// 이것들은 샘플 간에 일관된 값 사용을 위한 샘플 영역입니다.
/// 사용자는 자신의 요구에 따라 이를 지정해야 합니다.
///
/// 샘플로 제공된 예시 enum타입을 그대로 들고왔습니다.
/// 아직 이 enum 변수들이 무슨 역할을 하는지 모르는 상황이므로, 일단 옮겨서 사용되는 모습을 보고 분석할것입니다.
enum SamplePolyAreas
{
	SAMPLE_POLYAREA_GROUND,
	SAMPLE_POLYAREA_WATER,
	SAMPLE_POLYAREA_ROAD,
	SAMPLE_POLYAREA_DOOR,
	SAMPLE_POLYAREA_GRASS,
	SAMPLE_POLYAREA_JUMP
};
enum SamplePolyFlags
{
	SAMPLE_POLYFLAGS_WALK = 0x01,		// 걷기 능력 (지상, 풀, 도로)
	SAMPLE_POLYFLAGS_SWIM = 0x02,		// 수영 능력 (물)
	SAMPLE_POLYFLAGS_DOOR = 0x04,		// 문 통과 능력
	SAMPLE_POLYFLAGS_JUMP = 0x08,		// 점프 능력
	SAMPLE_POLYFLAGS_DISABLED = 0x10,	// 비활성화된 폴리곤
	SAMPLE_POLYFLAGS_ALL = 0xffff		// 모든 능력
};
enum SamplePartitionType
{
	SAMPLE_PARTITION_WATERSHED,
	SAMPLE_PARTITION_MONOTONE,
	SAMPLE_PARTITION_LAYERS
};

class Navigation
{
public:
	Navigation();
	~Navigation();

public:
	void Initialize();
	void Terminate();

	bool handleBuild();

private:
	class dtNavMesh* _navMesh;
	class dtNavMeshQuery* _navQuery;

	bool _keepInterResults;
	float _totalBuildTimeMs;

	unsigned char* _triareas;
	rcHeightfield* _solid;
	rcCompactHeightfield* _chf;
	rcContourSet* _cset;
	rcPolyMesh* _pmesh;
	rcConfig _cfg;
	rcPolyMeshDetail* _dmesh;
	rcContext* _ctx;

	unsigned char _navMeshDrawFlags;

	float _cellSize;
	float _cellHeight;
	float _agentHeight;
	float _agentRadius;
	float _agentMaxClimb;
	float _agentMaxSlope;
	float _regionMinSize;
	float _regionMergeSize;
	float _edgeMaxLen;
	float _edgeMaxError;
	float _vertsPerPoly;
	float _detailSampleDist;
	float _detailSampleMaxError;
	int _partitionType;

	bool _filterLowHangingObstacles;
	bool _filterLedgeSpans;
	bool _filterWalkableLowHeightSpans;
};                                             