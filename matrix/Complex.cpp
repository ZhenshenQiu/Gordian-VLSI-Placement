
//COMPLEX_CPP
#include <iostream>
#include <math.h>
#include <string.h>
#include "Complex.h"
Complex::Complex(double r,double i)
{
	Re=r;
	Im=i;
}

Complex::operator double()
{
return Re;
}
void Complex::setRe(double r)
{
	Re=r;
}
void Complex::setIm(double i)
{
	Im=i;
}
double Complex::getRe() const
{
	return Re;
}
double Complex::getIm() const
{
	return Im;
}
double Complex::getABS() const
{
	return sqrt(Re*Re+Im*Im);
}
void Complex::show() const
{
	if(Im==0)
		cout<<Re;
	if(Re==0&&Im!=0&&Im!=1&&Im!=-1)
		cout<<Im<<"i";
	if(Re==0&&Im==1.0)
		cout<<"i";
	if(Re==0&&Im==-1)
		cout<<"-i";
	if(Re!=0&&Im>0&&Im!=1.0)
		cout<<Re<<"+"<<Im<<"i";
	if(Re!=0&&Im==1.0)
		cout<<Re<<"+"<<"i";
	if(Re!=0&&Im<0&&Im!=-1)
		cout<<Re<<Im<<"i";
	if(Re!=0&&Im==-1)
		cout<<Re<<"-i";
}
const Complex &Complex::operator=(const Complex &temp)
{
	if(&temp!=this)//检查是否自我赋值
	{
		Re=temp.Re;
		Im=temp.Im;
	}
	return *this;
}
Complex Complex::con() const
{
	Complex temp(0,0);
	temp.Re=Re;
	temp.Im=-1*Im;
	return temp;
}
Complex Complex::operator+(Complex &z)
{
	Complex temp(0,0);
	temp.Re=Re+z.Re;
	temp.Im=Im+z.Im;
	return temp;
}
Complex Complex::operator-(Complex &z)
{
	Complex temp(0,0);
	temp.Re=Re-z.Re;
	temp.Im=Im-z.Im;
	return temp;
}
Complex Complex::operator*(Complex &z)
{
	Complex temp(0,0);
	temp.Re=Re*z.Re-Im*z.Im;
	temp.Im=Re*z.Im+Im*z.Re;
	return temp;
}
Complex Complex::operator/(Complex &z)
{
	Complex temp(0,0);
	if(z.Re==0&&z.Im==0)
	{
		cout<<"Error!除数不能为0!"<<endl;
		return temp;
	}		
	else
	{		
		temp.Re=((Re*z.Re+Im*z.Im)/(z.getABS()*z.getABS()));
		temp.Im=((Im*z.Re-Re*z.Im)/(z.getABS()*z.getABS()));
		return temp;
	}
}
int Complex::operator==(const Complex &z) const
{
	if(Re==z.Re&&Im==z.Im)
		return 1;
	else return 0;
}
int Complex::operator!=(const Complex &z) const
{
	if(Re!=z.Re||Im!=z.Im)
		return 1;
	else return 0;
}
ostream &operator<<(ostream &output,const Complex &z)
{
	if(z.Im==0)
		output<<z.Re;
	if(z.Re==0&&z.Im!=0&&z.Im!=1&&z.Im!=-1)
		output<<z.Im<<"i";
	if(z.Re==0&&z.Im==1)
		output<<"i";
	if(z.Re==0&&z.Im==-1)
		output<<"-i";
	if(z.Re!=0&&z.Im>0&&z.Im!=1)
		output<<z.Re<<"+"<<z.Im<<"i";
	if(z.Re!=0&&z.Im==1)
		output<<z.Re<<"+"<<"i";
	if(z.Re!=0&&z.Im<0&&z.Im!=-1)
		output<<z.Re<<z.Im<<"i";
	if(z.Re!=0&&z.Im==-1)
		output<<z.Re<<"-i";
	return output;
}
istream &operator>>(istream &input,Complex &z)
{
	int n=0,me=0,be=0,bi=1,mi=1,fe=-1,len=0;
	char *ptr;
	ptr=new char[100];
	do
	{
		input>>ptr[n];		
		if((ptr[n]=='+'||ptr[n]=='-')&&n!=0)
		{
			me=n;
			if(ptr[n]=='-')
					mi=-1;
		}
		if(ptr[n]=='i')
		{
			fe=n;
		}
		if(ptr[0]=='-')
		{
			be=1;
			bi=-1;
		}
		n++;
	}
	while(ptr[n-1]!=','&&ptr[n-1]!=';');
	ptr[n-1]='\0';
	len=strlen(ptr);
	if(fe==-1)
	{
		int i=0,p=len;
		double k=1.0,temp=0;
		for(i=0;i<len;i++)
		{
			if(ptr[i]=='.')
			{
				p=i;
				break;
			}
		}
		for(i=0;i<len-p-1;i++)
		{
			k=0.1*k;
		}
		for(i=len-1;i>=be;i--)
		{
			if(ptr[i]!='.')
			{
				temp=temp+k*(ptr[i]-48);
				k=k*10;
			}
		}
		z.Re=bi*temp;
		z.Im=0;
	}
	if(fe==me)				//i
	{
		z.Re=0;
		z.Im=1;
	}
	if(fe==1&&me==0)
	{
		if(be)				//-i;
		{
			z.Re=0;
			z.Im=-1;
		}
		if(!be)				//2i,3i,4i,5i,6i,7i,8i,9i;
		{
			z.Re=0;
			z.Im=ptr[0]-48;
		}
	}
	if(fe>1&&me==0)			//123i,-123i,1.23i,-1.23i;
	{
		int i=0,p=len;
		double k=1.0,temp=0.0;
		for(i=0;i<len;i++)
		{
			if(ptr[i]=='.')
			{
				p=i;
				break;
			}
		}
		for(i=0;i<fe-p-1;i++)
		{
			k=0.1*k;
		}
		for(i=fe-1;i>=be;i--)
		{
			if(ptr[i]!='.')
			{
				temp=temp+k*(ptr[i]-48);
				k=k*10;
			}
		}
		z.Re=0;
		z.Im=bi*temp;
	}
	if(me>0&&fe==me+1)			//1+i,1-i,123+i,-123-i,1.23+i,-1.23-i;
	{
		int i=0,p=len;
		double k=1.0,temp=0.0;
		for(i=0;i<len;i++)
		{
			if(ptr[i]=='.')
			{
				p=i;
				break;
			}
		}
		for(i=0;i<fe-p-1;i++)
		{
			k=0.1*k;
		}
		for(i=me-1;i>=be;i--)
		{	
			if(ptr[i]!='.')
			{
				temp=temp+k*(ptr[i]-48);
				k=k*10;
			}
		}
		z.Re=bi*temp;
		z.Im=mi;
	}
	if(me>0&&fe>me+1)			//123+123i,-123-123i,1.23+1.23i,-1.23-1.23i;
	{
		int i=0,p[2]={len,len};
		double k1=1.0,k2=1.0,temp1=0.0,temp2=0.0;
		for(i=0;i<len;i++)
		{
			if(ptr[i]=='.'&&i<me)
				p[0]=i;
			if(ptr[i]=='.'&&i>me)
				p[1]=i;
		}
		for(i=0;i<me-p[0]-1;i++)
		{
			k1=0.1*k1;
		}
		for(i=0;i<fe-p[1]-1;i++)
		{
			k2=0.1*k2;
		}
		for(i=me-1;i>=be;i--)
		{
			if(ptr[i]!='.')
			{
				temp1=temp1+k1*(ptr[i]-48);
				k1=k1*10;
			}
		}
		for(i=fe-1;i>me;i--)
		{
			if(ptr[i]!='.')
			{
				temp2=temp2+k2*(ptr[i]-48);
				k2=k2*10;
			}
		}
		z.Re=bi*temp1;
		z.Im=mi*temp2;
	}
	delete [] ptr;
	return input;
}
