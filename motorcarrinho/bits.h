#ifndef _BITS_H_
#define _BITS_H_



#define SET(BIT) (1 << BIT)
#define	SET_BIT(Y,BIT) (Y |= (BIT))
#define	CLR_BIT(Y,BIT) (Y &= ~(BIT))
#define CPL_BIT(Y,BIT) (Y ^= (BIT))
#define TST_BIT(Y,BIT) (Y & (BIT))

#endif /* BITS_H_ */
