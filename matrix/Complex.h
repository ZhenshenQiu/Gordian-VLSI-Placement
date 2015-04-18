//定义复数类 COMPLEX_H
#ifndef COMPLEX_H  //避免重复调用头文件
#define COMPLEX_H
using namespace std;

class Complex
{
	friend ostream &operator<<(ostream &,const Complex &);
   friend istream &operator>>(istream &,Complex &);        //输入复数时请以“,”隔开，以“;”结束。
public:
    Complex(double r=0,double i=0);
	void setRe(double r);//设定实部
	void setIm(double i);//设定虚部
	double getRe() const;//得到实部
	double getIm() const;//得到虚部
	double getABS() const;//得到复数的模
	void show() const;//显示复数
	const Complex &operator=(const Complex &);//赋值
	Complex con() const;//取共轭
	Complex operator+(Complex &z);//加
	Complex operator-(Complex &z);//减
	Complex operator*(Complex &z);//乘
	Complex operator/(Complex &z);//除
	operator double();
	int operator==(const Complex &z) const;//判断相等
	int operator!=(const Complex &z) const;//判断不等

private:
	double Re;
	double Im;
};
#endif