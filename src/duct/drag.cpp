#include "drag.h"
#include "bccell.h"
#include "jacrow.h"

namespace duct{
	namespace drag {
		using tasystem::JacRow;
		using std::make_tuple;
		using std::tuple;

		vd DragResistance::drag(const Cell& v) const {return vd(v.gc->Ns(),fillwith::zeros);}
		JacRow DragResistance::dDrag(const Cell& v) const {return JacRow(-1);}
		// dmat DragResistance::drhoi(const Cell& v) const {return v.zero;}
		// dmat DragResistance::dpi(const Cell& v) const {return v.zero;}

		d DragResistance::rho0(const Cell& v) {
			TRACE(15,"DragResistance::rho0()");
			d T0=v.T()(0);
			d p0=v.p()(0)+v.gc->p0();
			return v.gc->gas().rho(T0,p0);
		}
		d DragResistance::mu0(const Cell& v) {
			TRACE(15,"DragResistance::mu0()");
			d T0=v.T()(0);
			return v.gc->gas().mu(T0);
		}

		vd DragResistance::shearWaveNumber(const Cell& v) const {
			TRACE(15,"DragResistance::shearWaveNumber(const Cell& v)");
			const us Nf=v.gc->Nf();

			const d omg=v.gc->getomg();
			const vd omgvec=linspace(0,Nf*omg,Nf+1);
			d rh=v.rh();

			return rh*sqrt((rho0(v)/mu0(v))*omgvec);

		}
	} // namespace drag
} // namespace duct





















