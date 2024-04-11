#pragma once
#include <windows.h>
#include <vector>
#include <directxmath.h>

#include "DetourNavMesh.h"
#include "Recast.h"
#include "InputGeom.h"
#include "ChunkyTriMesh.h"
#include "DetourNavMeshQuery.h"
#include "DetourTileCache.h"

/// 이것들은 샘플 간에 일관된 값 사용을 위한 샘플 영역입니다.
/// 사용자는 자신의 요구에 따라 이를 지정해야 합니다.
///
/// 샘플로 제공된 예시 enum타입을 그대로 들고왔습니다.
/// 아직 이 enum 변수들이 무슨 역할을 하는지 모르는 상황이므로, 일단 옮겨서 사용되는 모습을 보고 분석할것입니다.
/// 

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

enum TestType
{
	TEST_PATHFIND,
	TEST_RAYCAST
};

static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

static const int TILECACHESET_MAGIC = 'T' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'TSET';
static const int TILECACHESET_VERSION = 1;
static const int EXPECTED_LAYERS_PER_TILE = 4;
static const int MAX_LAYERS = 32;

struct NavMeshSetHeader
{
	int magic;
	int version;
	int numTiles;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

struct Test
{
	Test() :
		type(),
		spos(),
		epos(),
		nspos(),
		nepos(),
		radius(0),
		includeFlags(0),
		excludeFlags(0),
		expand(false),
		straight(0),
		nstraight(0),
		polys(0),
		npolys(0),
		findNearestPolyTime(0),
		findPathTime(0),
		findStraightPathTime(0),
		next(0)
	{
	}

	~Test()
	{
		delete[] straight;
		delete[] polys;
	}

	TestType type;
	float spos[3];
	float epos[3];
	float nspos[3];
	float nepos[3];
	float radius;
	unsigned short includeFlags;
	unsigned short excludeFlags;
	bool expand;

	float* straight;
	int nstraight;
	dtPolyRef* polys;
	int npolys;

	int findNearestPolyTime;
	int findPathTime;
	int findStraightPathTime;

	Test* next;
private:
	// Explicitly disabled copy constructor and copy assignment operator.
	Test(const Test&);
	Test& operator=(const Test&);
};

// 이 PathFIndbox는 네비매쉬를 배열로 관리하기 편하도록 구성 요소들을 묶은 것
struct PathFindbox
{
	class dtNavMesh* _navMesh;
	class dtNavMeshQuery* _navQuery;

	// PathFind를 위해 필요한 부분들
	static const int MAX_POLYS = 256;
	static const int MAX_SMOOTH = 2048;

	dtPolyRef _startRef;
	dtPolyRef _endRef;
	dtPolyRef _parent[MAX_POLYS];
	float _startPos[3];
	float _endPos[3];
	dtQueryFilter _filter;
	dtPolyRef _path[MAX_POLYS];
	int _pathCount;
	float _polyPickExt[3];

	// Straight-pathfind를 위해 필요한 부분들
	float _straightPath[MAX_POLYS * 3];
	unsigned char _straightPathFlags[MAX_POLYS];
	dtPolyRef _straightPathPolys[MAX_POLYS];
	int _nstraightPath;

	// Raycast-pathfind를 위해 필요한 부분들
	dtRaycastHit _hit;
	dtPolyRef _RaycastPathPolys;
	float _hitPos[3];

	PathFindbox();
	~PathFindbox();
};

	class Navigation
	{
	private:
		Navigation();
		~Navigation();

	public:
		// 싱글턴 객체 반환 함수
		static Navigation& GetInstance();
		// 초기화 함수.
		void Initialize();
		// 제거 함수
		void Release();

		// path 에서 이미 빌드된 네비매쉬 파일을 읽어옵니다. .bin파일
		void LoadAll(const char* path, int index);
		// 네비매쉬를 직접 빌드 하기
		bool HandleBuild();
		// 네비매쉬를 업데이트 한다.
		void handleUpdate(const float dt);
		
		// 장애물을 추가한다.
		// pos = 장애물 위치 / radius = 장애물 크기 / height = 장애물 사이즈
		void addTempObstacle(const float* pos, float radius, float height);
		// 특정 장애물을 제거한다.
		void removeTempObstacle(const float* sp, const float* sq);
		// 모든 장애물을 제거한다.
		void clearAllTempObstacles();

		// 직선경로 탐색 함수
		std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> FindStraightPath(int index);
		// 이미 탐색된 경로를 가져오는 함수
		std::vector<std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3>> GetPath(int index);
		// Raycast 탐색 함수. (직선경로에 부딧히는게 있다면 거기까지만 경로 표시
		DirectX::XMFLOAT3 FindRaycastPath(int index);

		// startpos 와 endpos를 입력하는 함수. float[3] 버전
		void SetSEpos(int index, float sx, float sy, float sz, float ex, float ey, float ez);
		// startpos 와 endpos를 입력하는 함수. XMFLOAT3 버전
		void SetSEpos(int index, DirectX::XMFLOAT3 startPosition, DirectX::XMFLOAT3 endPosition);
		// startpos를 입력하는 함수. float[3] 버전
		void SetStartpos(int index, float x, float y, float z);
		// startpos를 입력하는 함수. XMFLOAT3 버전
		void SetStartpos(int index, DirectX::XMFLOAT3 position);
		// endpos를 입력하는 함수. float[3] 버전
		void SetEndpos(int index, float x, float y, float z);
		// endpos를 입력하는 함수. XMFLOAT3 버전
		void SetEndpos(int index, DirectX::XMFLOAT3 position);

	private:
		int rasterizeTileLayers(const int tx, const int ty, const rcConfig& cfg, struct TileCacheData* tiles, const int maxTiles);
		void getTilePos(const float* pos, int& tx, int& ty);
		void renderCachedTile(const int tx, const int ty, const int type);
		void renderCachedTileOverlay(const int tx, const int ty, double* proj, double* model, int* view);
		// sp, sq를 기반으로 장애물의 ref 값을 반환하는 함수. sp,sq가 뭔진 아직 나도 모르겠음
		dtObstacleRef hitTestObstacle(const dtTileCache* tc, const float* sp, const float* sq);
		static bool isectSegAABB(const float* sp, const float* sq,
								const float* amin, const float* amax,
								float& tmin, float& tmax);
	private:
		class InputGeom* _geom;
		PathFindbox _package[5];

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

		// solo_mesh -> temp+obstacle로 바꾸면서 추가된 부분
		struct LinearAllocator* _talloc;
		struct FastLZCompressor* _tcomp;
		struct MeshProcess* _tmproc;

		class dtTileCache* _tileCache;

		float _cacheBuildTimeMs;
		int _cacheCompressedSize;
		int _cacheRawSize;
		int _cacheLayerCount;
		unsigned int _cacheBuildMemUsage;

		enum DrawMode
		{
			DRAWMODE_NAVMESH,
			DRAWMODE_NAVMESH_TRANS,
			DRAWMODE_NAVMESH_BVTREE,
			DRAWMODE_NAVMESH_NODES,
			DRAWMODE_NAVMESH_PORTALS,
			DRAWMODE_NAVMESH_INVIS,
			DRAWMODE_MESH,
			DRAWMODE_CACHE_BOUNDS,
			MAX_DRAWMODE
		};

		DrawMode _drawMode;

		int _maxTiles;
		int _maxPolysPerTile;
		float _tileSize = 48;
	};