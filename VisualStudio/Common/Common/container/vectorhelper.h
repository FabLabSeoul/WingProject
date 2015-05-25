#pragma once


namespace common
{

	/**
	 @brief this function only avaible to unique ty value
	 remove ty and then rotate vector to sequence elements
	 */
	template <class Seq>
	bool removevector(Seq &seq, const typename Seq::value_type &ty)
	{
		for (size_t i=0; i < seq.size(); ++i)
		{
			if (seq[ i] == ty)
			{
				if ((seq.size()-1) > i) // elements를 회전해서 제거한다.
					std::rotate( seq.begin()+i, seq.begin()+i+1, seq.end() );
				seq.pop_back();
				return true;
			}
		}
		return false;
	}


	template <class Seq>
	void rotatepopvector(Seq &seq, const unsigned int idx)
	{
		if ((seq.size()-1) > idx) // elements를 회전해서 제거한다.
			std::rotate( seq.begin()+idx, seq.begin()+idx+1, seq.end() );
		seq.pop_back();
	}


}
