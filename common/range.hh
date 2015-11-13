#ifndef MYRANGE_HH
#define MYRANGE_HH


class Range
	{
	public:
		double beg,end;
		
		Range(double v):beg(v),end(v)
			{
			}
		double normalize(double v) const
			{
			if( v < std::min(beg,end)) v=std::min(beg,end);
			if( v > std::max(beg,end)) v=std::max(beg,end);
			return v;
			}
		
		Range(double beg,double end):beg(beg),end(end)
			 {
			 }
		
		double operator()(int n,int iterations) const
			{
			double v = beg+n*((end-beg)/((double)iterations));
			return normalize(v);
			}
		
		void reset(double b,double e)
			 {
			 beg=b;end=e;
			 }
		void reset(double v)
			 {
			 reset(v,v);
			 }
		
	};



#endif
