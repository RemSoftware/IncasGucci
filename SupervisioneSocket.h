//---------------------------------------------------------------------------
#ifndef SupervisioneSocketH
#define SupervisioneSocketH

//---------------------------------------------------------------------------

#include <scktcomp.hpp>
#include "eventi.h"
#include "socket.h"

class SUPERVISIONEMISS_STATESOCKET : public MISS_STATESOCKET {
	public:
		SUPERVISIONEMISS_STATESOCKET();
	private:
		void Elabora(char *ev, TCustomWinSocket *Socket);
};

#endif

