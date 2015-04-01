// box
#pragma once

namespace graphic
{
	// AABB Box
	struct Box
	{
		Box();

		void SetBox( const Vector3 &vMin, const Vector3 &vMax );
		void GetMinMax( OUT Vector3 &vMin, OUT Vector3 &vMax, bool isOwn=false, bool isUpdate=false ) ;
		float GetSize() const;
		void Update();
		bool Collision( const Box &box ) const;
		void SetWorldTM( const Matrix44 &mat );
		void MultiplyWorldTM( const Matrix44 &mat );
		void Render() const;
		void Render_Hierarchy() const;
		vector<Vector3>& GetBoxVertices() { return m_box; }


		Vector3 m_min, m_max; // Box의 최대 최소값
		vector<Vector3> m_box;
		Matrix44 m_matWorld;
	};

}
