#include<stdio.h>
#include<iostream>
#include<gsl/gsl_errno.h>
#include<gsl/gsl_matrix.h>
#include<gsl/gsl_odeiv2.h>
#include<gsl/gsl_sf_bessel.h>
#include<fstream>
#include<cmath>
#include<vector>
#include<complex>

void InputData(int & numeq, double & coupling,double & Energydif, double & rkstep,
		double & alpha, int & nmax, double & ti, double & tf , int & moments ,double & omega );


std::complex<double> I(0.0,1.0);

std::ofstream out("de10omega2n30a4.dat");
std::ifstream input("parameters.dat");

struct parameters {
	int numeq ; //number of amplitudes equations
	double coupling ;
	double Energydif ;
	double besJo ;
	std::vector<double> besJn;
	double omega;
	int nmax;
};

int func (double t, const double y[], double dydt[],  void *p)
{
	struct parameters *my_param_pointer= (parameters *)p;
	int numeq = my_param_pointer->numeq;
	double Energydif = my_param_pointer->Energydif;
	double coupling = my_param_pointer->coupling;
	double J0= my_param_pointer->besJo;
	std::vector<double> bvec=my_param_pointer->besJn;
	double omega=my_param_pointer->omega;
	int nmax=my_param_pointer->nmax;

	int dim = numeq;
	std::complex<double> dcdt[dim], c[dim];

		for(int i=0; i<dim; ++i)
		{
			c[i]=0.0+I*0.0;
			dcdt[i]=0.0+I*0.0;
			c[i]=y[2*i]+I*y[2*i+1];
		}
		double omegaR{omega/coupling};
		//std::cout<<"fun= "<<numeq<<" "<<coupling<<" "<< Energydif<<" "<<omega<<" "<<nmax<<'\n';
		std::complex<double> sum1{J0,0},sum2{J0,0};


		for(int n{0};n<nmax;++n){
			double nd=1.0*(n+1);
			sum1 += bvec[n]*exp(nd*I*omegaR*t); //pros8hkh 8etikou n
			sum1 += bvec[n]*std::pow(-1,nd)*exp(-nd*I*omegaR*t); //pros8hkh arnhtikou n
			sum2 += bvec[n]*std::pow(-1,nd)*exp(nd*I*omegaR*t);
			sum2 += bvec[n]*exp(-nd*I*omegaR*t);
		}

		//std::cout<< t<< sum1<<"  "<<sum2<<'\n';

	dcdt[0] = +I*coupling*sum1*c[1];
	dcdt[1] = -I*Energydif*c[1]+I*coupling*sum2*c[0];

	for(int i=0; i<dim; ++i)
	{
		dydt[2*i]=real(dcdt[i]);
		dydt[2*i+1]=imag(dcdt[i]);
	}

	return GSL_SUCCESS;

}


int
jac (double t, const double y[], double *dfdy,
     double dfdt[], void *params)
{
	return GSL_SUCCESS;
}

int main (void)
{
	int numeq,n, nmax, moments; //n -number of real equations
	double coupling , Energydif ,rkstep,alpha,t,tf,omega;
	InputData(numeq, coupling, Energydif,	rkstep,  alpha, nmax, t, tf, moments,omega);
	//std::cout<<"input= "<<numeq<<" "<<coupling<<" "<< Energydif<<" "<<omega<<" "<<nmax<<'\n';

	std::vector<double> besvec(nmax,0.0);
	int bes=gsl_sf_bessel_Jn_array (1, nmax, alpha, &besvec[0] );
	double J0= gsl_sf_bessel_J0(alpha);

	//se monades tou coupling -nValue
	double ncoupling{coupling/coupling};
	double nEnergydif{Energydif/coupling};

	struct parameters my_point = {numeq,ncoupling,nEnergydif,J0,besvec,omega,nmax};  //den evgale error
	//std::cout<<"main= "<<numeq<<" "<<coupling<<" "<< Energydif<<" "<<omega<<" "<<nmax<<'\n';


	n=2*numeq; // number of real equations

	const gsl_odeiv2_step_type * T  = gsl_odeiv2_step_rk8pd;  // embeded RK
	gsl_odeiv2_system sys = {func, jac, n, &my_point};
	gsl_odeiv2_driver * d= gsl_odeiv2_driver_alloc_y_new(&sys,gsl_odeiv2_step_rk8pd,rkstep,rkstep,0.0);//(..initial step size,absolute error, relative error)   This function creates a new control object which will keep the local error on each step within an absolute error of eps_abs and relative error

	std::vector<double> y(n,0.0);
	y[0] = 1.0;

	double tinit = t;
	for (int i=1; i<=moments; ++i)	{
		double twant=tinit+i*tf/moments;

		int status = gsl_odeiv2_driver_apply (d, &t, twant, &y[0]);
		
		if (status != GSL_SUCCESS)
			break;
		
		double pop0=y[0]*y[0]+y[1]*y[1];
		double pop1=y[2]*y[2]+y[3]*y[3];
		double pop=pop0+pop1;
		if (std::abs(pop-1)>1.0E-04)
		{
			std::cerr<<" Total Popoulation>1 ! "<<std::endl;
			std::exit(1);
		}

		out<<twant<<"  "<<pop0<< "  "<< pop1<<"  "<<pop<<std::endl;
    }
	
	gsl_odeiv2_driver_free (d);
	return 0;
}

void InputData(int & numeq, double & coupling, double & Energydif,
		double & rkstep, double & alpha, int & nmax, double & ti, double & tf , int & moments, double & omega)
{
	if(!input.is_open())
	{
		std::cerr<<".....Failed to Open Data File!!!!!"<<std::endl;
		std::exit(1); //exei shmasia to 1 einai kai pio katv?
	}

	std::string st1, st2, st3, st4, st5, st6,st7,st8, st9, st10;
	getline(input, st1);
	getline(input, st2);
	getline(input, st3);
	getline(input, st4);
	getline(input, st5);
	getline(input, st6);
	getline(input, st7);
	getline(input, st8);
	getline(input, st9);
	getline(input, st10);

	std::cout<<"String input ";
	if(st1.find(":",0) != std::string::npos)
		if(st2.find(":",0) != std::string::npos)
			if(st3.find(":",0) != std::string::npos)
				if(st4.find(":",0) != std::string::npos)
					if(st5.find(":",0) != std::string::npos)
						if(st6.find(":",0) != std::string::npos)
							if(st7.find(":",0) != std::string::npos)
								if(st8.find(":",0) != std::string::npos)
									if(st9.find(":",0) != std::string::npos)
										if(st10.find(":",0) != std::string::npos)
							std::cout<<"successful!"<<std::endl;
	std::cout<<std::endl;

	st1.erase(0,st1.find(":",0)+1);
	st2.erase(0,st2.find(":",0)+1);
	st3.erase(0,st3.find(":",0)+1);
	st4.erase(0,st4.find(":",0)+1);
	st5.erase(0,st5.find(":",0)+1);
	st6.erase(0,st6.find(":",0)+1); //1 h 2?
	st7.erase(0,st7.find(":",0)+1);
	st8.erase(0,st8.find(":",0)+1);
	st9.erase(0,st9.find(":",0)+1);
	st10.erase(0,st10.find(":",0)+1);

	numeq =  atoi(&st1[0]);
	coupling =  atof(&st2[0]);
	Energydif = atof(&st3[0]);
	rkstep= atof(&st4[0]);
	alpha = atof(&st5[0]); // J(alpha)
	nmax = atoi(&st6[0]); //sth bessel
	ti= atof(&st7[0]);
	tf= atof(&st8[0]);
	moments= atoi(&st9[0]); //sunolo xronikwn stigmwn
	omega= atof(&st10[0]);

}






