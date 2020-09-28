// Generalized functor implementation helpers. 
// Copyright Aleksei Trunov 2005 
// Use, copy, modify, distribute and sell it for free.
// "Yet Another Generalized Functors Implementation in C++"
// http://www.codeproject.com/cpp/genfunctors.asp
// rg [5/25/06] - Dunno if we can use this. We can always use Boost's functors once we port it to Xbox (but Boost's aren't as efficient).

#ifndef _FUNBIND_H_
#define _FUNBIND_H_

#include "Functor.h"

namespace GF {

// ----------------------------------------------------------------------------

template <class TL, class IdsTL, bool include> 
struct Filter2
{
	template <class TL2, int i, class IdsTL2, template <class, int> class Predicate> struct Temp
	{
		typedef typename GU::Select<
			Predicate<typename IdsTL2::Head, i>::value, 
			typename GU::TypeList<typename TL2::Head, typename Temp<typename TL2::Tail, i+1, typename IdsTL2::Tail, Predicate>::Result>,
			typename Temp<typename TL2::Tail, i+1, IdsTL2, Predicate>::Result
		>::Result Result;
	};
	template <class TL2, int i, template <class, int> class Predicate> 
	struct Temp<TL2, i, GU::NullType, Predicate>
	{
		typedef typename GU::Select<include, GU::NullType, TL2>::Result Result;
	};
	template <int i, class IdsTL2, template <class, int> class Predicate> 
	struct Temp<GU::NullType, i, IdsTL2, Predicate>
	{
		typedef GU::NullType Result;
	};
	template <int i, template <class, int> class Predicate> 
	struct Temp<GU::NullType, i, GU::NullType, Predicate>
	{
		typedef GU::NullType Result;
	};
	template <class TL2> struct Temp<TL2, 0, GU::NullType, GU::IsIntType>
	{
		typedef GU::NullType Result;
	};
	template <class TL2> struct Temp<TL2, 0, typename GU::IdsFromTL<TL2>::Type, GU::IsIntType>
	{
		typedef TL2 Result;
	};
	template <class TL2> struct Temp<TL2, 0, GU::NullType, GU::NotIntType>
	{
		typedef TL2 Result;
	};
	template <class TL2> struct Temp<TL2, 0, typename GU::IdsFromTL<TL2>::Type, GU::NotIntType>
	{
		typedef GU::NullType Result;
	};
	typedef typename GU::Select<
		include, 
		typename Temp<TL, 0, IdsTL, GU::IsIntType>::Result,
		typename Temp<TL, 0, IdsTL, GU::NotIntType>::Result
	>::Result Result;
};

template <class TL, class IdsTL> struct BoundTL2
{
	typedef typename Filter2<TL, IdsTL, true>::Result Result;
};
template <class TL, class IdsTL> struct UnboundTL2
{	
	typedef typename Filter2<TL, IdsTL, false>::Result Result;
};

// ----------------------------------------------------------------------------

template <class Incoming, class IdsTL> struct BoundHelper
{
	typedef typename Incoming::TypeListType IncomingTL;
	typedef typename BoundTL2<IncomingTL, IdsTL>::Result BoundTL;
	typedef typename GU::TypeAtNonStrict<BoundTL, 0, GU::NullType>::Result Parm1;
	typedef typename GU::TypeAtNonStrict<BoundTL, 1, GU::NullType>::Result Parm2;
	typedef typename GU::TypeAtNonStrict<BoundTL, 2, GU::NullType>::Result Parm3;
	typedef typename GU::TypeAtNonStrict<BoundTL, 3, GU::NullType>::Result Parm4;
	typedef typename GU::TypeAtNonStrict<BoundTL, 4, GU::NullType>::Result Parm5;
};
template <class Incoming, class IdsTL> struct UnboundHelper
{
	typedef typename Incoming::TypeListType IncomingTL;
	typedef typename UnboundTL2<IncomingTL, IdsTL>::Result UnboundTL;
	typedef typename GU::TypeAtNonStrict<UnboundTL, 0, GU::NullType>::Result Parm1;
	typedef typename GU::TypeAtNonStrict<UnboundTL, 1, GU::NullType>::Result Parm2;
	typedef typename GU::TypeAtNonStrict<UnboundTL, 2, GU::NullType>::Result Parm3;
	typedef typename GU::TypeAtNonStrict<UnboundTL, 3, GU::NullType>::Result Parm4;
	typedef typename GU::TypeAtNonStrict<UnboundTL, 4, GU::NullType>::Result Parm5;
};

template <int i, class BoundPTL, class UnboundPTL, class IdsTL, class TL> struct MergeParmsH
{
	typedef GU::InstantiateH<TL, GU::TupleHolder, i> ResultType;
	struct Bound
	{
		static inline ResultType MergeParms(BoundPTL const& bound, UnboundPTL const& unbound)
		{
			return ResultType(
				static_cast<typename BoundPTL::LeftBase const&>(bound).value, 
				MergeParmsH<i+1, typename BoundPTL::RightBase, UnboundPTL, typename IdsTL::Tail, typename TL::Tail>::MergeParms(static_cast<typename BoundPTL::RightBase const&>(bound), unbound)
			); 
		}
	};
	struct Unbound
	{
		static inline ResultType MergeParms(BoundPTL const& bound, UnboundPTL const& unbound)
		{
			return ResultType(
				static_cast<typename UnboundPTL::LeftBase const&>(unbound).value, 
				MergeParmsH<i+1, BoundPTL, typename UnboundPTL::RightBase, IdsTL, typename TL::Tail>::MergeParms(bound, static_cast<typename UnboundPTL::RightBase const&>(unbound))
			); 
		}
	};
	template <int j, class IdsTL2> struct Predicate { enum { value = IdsTL2::Head::value == j }; };
	template <int j> struct Predicate<j, GU::NullType> { enum { value = false }; };
	static inline ResultType MergeParms(BoundPTL const& bound, UnboundPTL const& unbound)
	{
		return GU::Select<
			Predicate<i, IdsTL>::value,
			Bound,
			Unbound
		>::Result::MergeParms(bound, unbound); 
	}
};
template <int i, class BoundPTL, class UnboundPTL, class IdsTL> 
struct MergeParmsH<i, BoundPTL, UnboundPTL, IdsTL, GU::NullType>
{
	static inline GU::NullType MergeParms(BoundPTL const&, UnboundPTL const&) 
	{ 
		return GU::NullType(); 
	}
};
template <class BoundPTL, class UnboundPTL, class TL> 
struct MergeParmsH<0, BoundPTL, UnboundPTL, GU::NullType, TL>
{
	static inline typename CallParms<TL>::ParmsListType 
	MergeParms(BoundPTL const&/* bound*/, UnboundPTL const& unbound) 
	{ 
		return unbound; 
	}
};

template <class TL, class IdsTL, class BoundPTL, class UnboundPTL> inline 
typename CallParms<TL>::ParmsListType MergeParms(BoundPTL const& bound, UnboundPTL const& unbound)
{
	return MergeParmsH<0, BoundPTL, UnboundPTL, IdsTL, TL>::MergeParms(bound, unbound);
}

template <typename Incoming, typename BoundIdsTL> class Binder
{
public:
	typedef typename Incoming::ResultType ResultType;
	typedef typename Incoming::TypeListType TypeListType;
	typedef typename BoundHelper<Incoming, BoundIdsTL>::BoundTL BoundParamsTL;
	typedef typename UnboundHelper<Incoming, BoundIdsTL>::UnboundTL UnboundParamsTL;
	typedef typename CallParms<BoundParamsTL>::ParmsListType BoundPTL;
	typedef typename CallParms<UnboundParamsTL>::ParmsListType UnboundPTL;
	Binder() : fun_(), bound_() {}
    Binder(Incoming fun, BoundPTL bound) : fun_(fun), bound_(bound) {}
	//typedef Functor<typename Incoming::ResultType, UnboundParamsTL, typename Incoming::Checking, Incoming::size> Outgoing;
	typedef Functor<typename Incoming::ResultType, UnboundParamsTL> Outgoing;
	typedef typename Incoming::ResultType ResultType;
	typedef typename UnboundHelper<Incoming, BoundIdsTL>::Parm1 Parm1;
	typedef typename UnboundHelper<Incoming, BoundIdsTL>::Parm2 Parm2;
	typedef typename UnboundHelper<Incoming, BoundIdsTL>::Parm3 Parm3;
	typedef typename UnboundHelper<Incoming, BoundIdsTL>::Parm4 Parm4;
	typedef typename UnboundHelper<Incoming, BoundIdsTL>::Parm5 Parm5;
	inline ResultType operator()() const 
	{ 
		return fun_(bound_);	// little hand-optimization
		//return fun_(BoundPTL(bound_)); 
		//return fun_(MergeParms<TypeListType, BoundIdsTL>(bound_, CallParms<UnboundParamsTL>::Make())); 
	}
	inline ResultType operator()(Parm1 p1) const 
	{ 
		return fun_(MergeParms<TypeListType, BoundIdsTL>(bound_, CallParms<UnboundParamsTL>::Make(p1))); 
	}
	inline ResultType operator()(Parm1 p1, Parm2 p2) const 
	{ 
		return fun_(MergeParms<TypeListType, BoundIdsTL>(bound_, CallParms<UnboundParamsTL>::Make(p1, p2))); 
	}
	inline ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) const 
	{ 
		return fun_(MergeParms<TypeListType, BoundIdsTL>(bound_, CallParms<UnboundParamsTL>::Make(p1, p2, p3))); 
	}
	inline ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const 
	{ 
		return fun_(MergeParms<TypeListType, BoundIdsTL>(bound_, CallParms<UnboundParamsTL>::Make(p1, p2, p3, p4))); 
	}
	inline ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5) const 
	{ 
		return fun_(MergeParms<TypeListType, BoundIdsTL>(bound_, CallParms<UnboundParamsTL>::Make(p1, p2, p3, p4, p5))); 
	}
private:
    Incoming fun_;
    BoundPTL bound_;
};

// ----------------------------------------------------------------------------

template <class Incoming> inline 
typename Binder<Incoming, typename GU::CreateIdsTL<>::Type>::Outgoing
Bind(
	Incoming const& fun
) {
	typedef typename GU::CreateIdsTL<>::Type BoundIdsTL;
	typedef typename Binder<Incoming, BoundIdsTL>::Outgoing Outgoing;
	return Outgoing(Binder<Incoming, BoundIdsTL>(fun, CallParms<typename BoundHelper<Incoming, BoundIdsTL>::BoundTL>::Make()));
}

template <int i1, class Incoming> inline 
typename Binder<Incoming, typename GU::CreateIdsTL<i1>::Type>::Outgoing
Bind(
	Incoming const& fun
	, typename BoundHelper<Incoming, typename GU::CreateTL<GU::Int2Type<i1> >::Type>::Parm1 p1
) {
	typedef typename GU::CreateIdsTL<i1>::Type BoundIdsTL;
	typedef typename Binder<Incoming, BoundIdsTL>::Outgoing Outgoing;
	return Outgoing(Binder<Incoming, BoundIdsTL>(fun, CallParms<typename BoundHelper<Incoming, BoundIdsTL>::BoundTL>::Make(p1)));
}

template <int i1, int i2, class Incoming> inline 
typename Binder<Incoming, typename GU::CreateIdsTL<i1, i2>::Type>::Outgoing
Bind(
	Incoming const& fun
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2>::Type>::Parm1 p1
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2>::Type>::Parm2 p2
) {
	typedef typename GU::CreateIdsTL<i1, i2>::Type BoundIdsTL;
	typedef typename Binder<Incoming, BoundIdsTL>::Outgoing Outgoing;
	return Outgoing(Binder<Incoming, BoundIdsTL>(fun, CallParms<typename BoundHelper<Incoming, BoundIdsTL>::BoundTL>::Make(p1, p2)));
}

template <int i1, int i2, int i3, class Incoming> inline 
typename Binder<Incoming, typename GU::CreateIdsTL<i1, i2, i3>::Type>::Outgoing
Bind(
	Incoming const& fun
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3>::Type>::Parm1 p1
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3>::Type>::Parm2 p2
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3>::Type>::Parm3 p3
) {
	typedef typename GU::CreateIdsTL<i1, i2, i3>::Type BoundIdsTL;
	typedef typename Binder<Incoming, BoundIdsTL>::Outgoing Outgoing;
	return Outgoing(Binder<Incoming, BoundIdsTL>(fun, CallParms<typename BoundHelper<Incoming, BoundIdsTL>::BoundTL>::Make(p1, p2, p3)));
}

template <int i1, int i2, int i3, int i4, class Incoming> inline 
typename Binder<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4>::Type>::Outgoing
Bind(
	Incoming const& fun
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4>::Type>::Parm1 p1
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4>::Type>::Parm2 p2
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4>::Type>::Parm3 p3
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4>::Type>::Parm4 p4
) {
	typedef typename GU::CreateIdsTL<i1, i2, i3, i4>::Type BoundIdsTL;
	typedef typename Binder<Incoming, BoundIdsTL>::Outgoing Outgoing;
	return Outgoing(Binder<Incoming, BoundIdsTL>(fun, CallParms<typename BoundHelper<Incoming, BoundIdsTL>::BoundTL>::Make(p1, p2, p3, p4)));
}

template <int i1, int i2, int i3, int i4, int i5, class Incoming> inline 
typename Binder<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5>::Type>::Outgoing
Bind(
	Incoming const& fun
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5>::Type>::Parm1 p1
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5>::Type>::Parm2 p2
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5>::Type>::Parm3 p3
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5>::Type>::Parm4 p4
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5>::Type>::Parm5 p5
) {
	typedef typename GU::CreateIdsTL<i1, i2, i3, i4, i5>::Type BoundIdsTL;
	typedef typename Binder<Incoming, BoundIdsTL>::Outgoing Outgoing;
	return Outgoing(Binder<Incoming, BoundIdsTL>(fun, CallParms<typename BoundHelper<Incoming, BoundIdsTL>::BoundTL>::Make(p1, p2, p3, p4, p5)));
}

template <int i1, int i2, int i3, int i4, int i5, int i6, class Incoming> inline 
typename Binder<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6>::Type>::Outgoing
Bind(
	Incoming const& fun
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6>::Type>::Parm1 p1
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6>::Type>::Parm2 p2
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6>::Type>::Parm3 p3
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6>::Type>::Parm4 p4
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6>::Type>::Parm5 p5
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6>::Type>::Parm5 p6
) {
	typedef typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6>::Type BoundIdsTL;
	typedef typename Binder<Incoming, BoundIdsTL>::Outgoing Outgoing;
	return Outgoing(Binder<Incoming, BoundIdsTL>(fun, CallParms<typename BoundHelper<Incoming, BoundIdsTL>::BoundTL>::Make(p1, p2, p3, p4, p5, p6)));
}

template <int i1, int i2, int i3, int i4, int i5, int i6, int i7, class Incoming> inline 
typename Binder<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6, i7>::Type>::Outgoing
Bind(
	Incoming const& fun
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6, i7>::Type>::Parm1 p1
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6, i7>::Type>::Parm2 p2
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6, i7>::Type>::Parm3 p3
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6, i7>::Type>::Parm4 p4
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6, i7>::Type>::Parm5 p5
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6, i7>::Type>::Parm5 p6
	, typename BoundHelper<Incoming, typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6, i7>::Type>::Parm5 p7
) {
	typedef typename GU::CreateIdsTL<i1, i2, i3, i4, i5, i6, i7>::Type BoundIdsTL;
	typedef typename Binder<Incoming, BoundIdsTL>::Outgoing Outgoing;
	return Outgoing(Binder<Incoming, BoundIdsTL>(fun, CallParms<typename BoundHelper<Incoming, BoundIdsTL>::BoundTL>::Make(p1, p2, p3, p4, p5, p6, p7)));
}

}

#endif // _FUNBIND_H_
