
#pragma once


namespace common
{

	struct sRect : public tagRECT
	{
		sRect();
		sRect(int left, int top, int right, int bottom);
		static sRect Rect(int x, int y, int width, int height);

		void SetX(const int x);
		void SetY(const int y);
		void SetWidth(const int width);
		void SetHeight(const int height);

		int Width() const;
		int Height() const;
	};

}
