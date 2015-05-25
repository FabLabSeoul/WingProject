// C3DPhysxDialog.cpp : implementation file
//

#include "stdafx.h"
#include "C3DPhysxView.h"
#include "afxdialogex.h"



using namespace graphic;
using namespace physx;

C3DPhysxView *g_3DPhysxView = NULL;


// C3DPhysxDialog dialog
C3DPhysxView::C3DPhysxView(CWnd* pParent /*=NULL*/)
	: C3DView(pParent)
{
	g_3DPhysxView = this;
}

C3DPhysxView::~C3DPhysxView()
{
	SAFE_RELEASE2(m_Scene);
	SAFE_RELEASE2(m_CpuDispatcher);
	SAFE_RELEASE2(m_physics);
	SAFE_RELEASE2(m_Foundation);

	if (SCRATCH_BLOCK_SIZE)
		SAFE_DELETEA(m_ScratchBlock);

	for (u_int i = 0; i < m_cubes.size(); ++i)
		SAFE_DELETE(m_cubes[i]);
	m_cubes.clear();
}

void C3DPhysxView::DoDataExchange(CDataExchange* pDX)
{
	C3DView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(C3DPhysxView, C3DView)
	ON_BN_CLICKED(IDOK, &C3DPhysxView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &C3DPhysxView::OnBnClickedCancel)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// C3DPhysxDialog message handlers

BOOL C3DPhysxView::OnInitDialog()
{
	if (!C3DView::OnInitDialog())
		return FALSE;

	if (!InitPhysX())
	{
		::AfxMessageBox(L"PhysX Initialize Error!!");
		return 0;
	}

	m_cubes.reserve(100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


// PhysX 초기화.
bool C3DPhysxView::InitPhysX()
{
	//This certainly has a performance and memory profile effect and thus should be used
	//only in non-production builds.
	bool recordMemoryAllocations = true;


	static physx::PxDefaultErrorCallback gDefaultErrorCallback;
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_pxAllocator, gDefaultErrorCallback);
	if (!m_Foundation)
		return false;

	PxTolerancesScale scale;
	//customizeTolerances(scale);

	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, scale, recordMemoryAllocations);
	if (!m_physics)
		return false;

	if (!PxInitExtensions(*m_physics))
		return false;

	m_Material = m_physics->createMaterial(0.5f, 0.5f, 0.1f);


	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.flags |= PxSceneFlag::eREQUIRE_RW_LOCK;


	if (!sceneDesc.cpuDispatcher)
	{
		m_CpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		if (!m_CpuDispatcher)
			return false;
		//fatalError("PxDefaultCpuDispatcherCreate failed!");
		sceneDesc.cpuDispatcher = m_CpuDispatcher;
	}

	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVETRANSFORMS;

	m_Scene = m_physics->createScene(sceneDesc);
	if (!m_Scene)
		return false;
	//fatalError("createScene failed!");

	m_ScratchBlock = new BYTE[SCRATCH_BLOCK_SIZE];

	//createGrid();

	//{
	//	sPhysxBox *box = new sPhysxBox;
	//	box->rigid = CreateBox(PxVec3(0, 10, 0), PxVec3(1, 1, 10), &PxVec3(0, 0, 0), 1);
	//	box->rigid->setAngularDamping(0);
	//	box->rigid->setLinearDamping(0);
	//	box->cube.SetCube(Vector3(-1, -1, -10), Vector3(1, 1, 10));
	//	Matrix44 mat;
	//	mat.SetTranslate(Vector3(0, 10, 0));
	//	box->cube.SetTransform(mat);
	//	box->cube.GetMaterial().InitBlue();
	//	m_cubes.push_back(box);

	//	PxTransform tm0;
	//	PxTransform tm1(0, 1, 0,
	//		PxQuat(ANGLE2RAD(0), PxVec3(0, 1, 0)));

	//	tm1.createIdentity();
	//	if (PxRevoluteJoint*j = PxRevoluteJointCreate(*m_physics,
	//		NULL, PxTransform(0, 5, 0),
	//		box->rigid, tm1)
	//		)
	//	{
	//		const PxReal tolerance = 0.2f;
	//		j->setProjectionLinearTolerance(tolerance);
	//	}
	//}

	return true;
}


// 그리드 평면 생성
PxRigidActor* C3DPhysxView::CreateGrid()
{
	PxSceneWriteLock scopedLock(*m_Scene);
	//Renderer* renderer = getRenderer();
	//PX_ASSERT(renderer);

	PxRigidStatic* plane = PxCreatePlane(*m_physics, PxPlane(PxVec3(0, 1, 0), 0), *m_Material);
	if (!plane)
		return NULL;
	//fatalError("create plane failed!");

	m_Scene->addActor(*plane);

	//PxShape* shape;
	//plane->getShapes(&shape, 1);

	//RenderGridActor* actor = SAMPLE_NEW(RenderGridActor)(*renderer, 20, 10.0f);
	//link(actor, shape, plane);
	//actor->setTransform(PxTransform(PxIdentity));
	//mRenderActors.push_back(actor);
	//actor->setRenderMaterial(material);
	return plane;
}


PX_FORCE_INLINE void SetupDefaultRigidDynamic(PxRigidDynamic& body, bool kinematic = false)
{
	body.setActorFlag(PxActorFlag::eVISUALIZATION, true);
	body.setAngularDamping(0.5f);
	body.setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, kinematic);
}


// 박스 생성
PxRigidDynamic* C3DPhysxView::CreateBox(const PxVec3& pos, const PxVec3& dims, const PxVec3* linVel, PxReal density)
{
	PxSceneWriteLock scopedLock(*m_Scene);
	PxRigidDynamic* box = PxCreateDynamic(*m_physics, PxTransform(pos), PxBoxGeometry(dims), *m_Material, density);
	PX_ASSERT(box);

	SetupDefaultRigidDynamic(*box);
	m_Scene->addActor(*box);
	//addPhysicsActors(box);

	if (linVel)
		box->setLinearVelocity(*linVel);

	//createRenderObjectsFromActor(box, material);
	return box;
}


// 애니메이션 전 처리
void C3DPhysxView::UpdateBefore(const float deltaSeconds)
{
	{
		// PhysX 물리 연산.
		PxSceneWriteLock writeLock(*m_Scene);
		m_Scene->simulate(m_IncSeconds);// , 0, m_ScratchBlock, SCRATCH_BLOCK_SIZE);
	}

	for (u_int i = 0; i < m_cubes.size(); ++i)
	{
		PxTransform tm = m_cubes[i]->rigid->getGlobalPose();
		Matrix44 tm1;
		tm1.SetTranslate(*(Vector3*)&tm.p);
		Quaternion q(tm.q.x, tm.q.y, tm.q.z, tm.q.w);
		Matrix44 tm2 = q.GetMatrix();
		m_cubes[i]->cube.SetTransform(tm2 * tm1);
	}

}


// 애니메이션 후 처리
void C3DPhysxView::UpdateAfter(const float deltaSeconds)
{
	m_Scene->flushSimulation();
	m_Scene->fetchResults(true);// PX_RIGID_BODY_FINISHED, true);
}


// 출력
void C3DPhysxView::Render()
{
	RET(!m_isInitDx);

	//화면 청소
	if (SUCCEEDED(GetDevice()->Clear(
		0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_XRGB(150, 150, 150), 1.0f, 0)))
	{
		GetDevice()->BeginScene();

		cLightManager::Get()->GetMainLight().Bind(0);

		// 백그라운드 그리드, 축 출력.
		GetRenderer()->RenderGrid();
		GetRenderer()->RenderAxis();

		for (u_int i = 0; i < m_cubes.size(); ++i)
		{
			m_cubes[i]->cube.Render(Matrix44::Identity);
		}


		// 파생받는 클래스에서 구현한다.
		RenderChild();


		GetRenderer()->RenderFPS();

		GetDevice()->EndScene();
		GetDevice()->Present(NULL, NULL, NULL, NULL);
	}
}


void C3DPhysxView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (0)
	{
		const Vector3 pos = common::GetRandomVector();
		//const float x = (rand() % 100) * 0.02f;
		Vector3 newPos = pos * 10;// +Vector3(0, 0, 5);
		newPos.y = 10;
		newPos.x = 0;

		sPhysxBox *box = new sPhysxBox;
		box->rigid = CreateBox(PxVec3(newPos.x, newPos.y, newPos.z), PxVec3(1, 1, 1), &PxVec3(0, 0, 0), 1);
		box->cube.SetCube(Vector3(-1, -1, -1), Vector3(1, 1, 1));
		Matrix44 mat;
		mat.SetTranslate(newPos);
		box->cube.SetTransform(mat);

		Vector3 color = GetRandomVector();
		color += Vector3(1, 1, 1);
		color /= 2.f;
		box->cube.GetMaterial().Init(color * 0.4f, color*0.9f, Vector4(1, 1, 1, 1));
		m_cubes.push_back(box);
	}
	else
	{
		//PxRigidBodyExt::addLocalForceAtPos(*m_cubes[0]->rigid, PxVec3(0, 1000, 0), PxVec3(0, 10, 5));
	}


	SetCapture();
	SetFocus();
	m_LButtonDown = true;
	m_curPos = point;
	C3DView::OnLButtonDown(nFlags, point);
}

