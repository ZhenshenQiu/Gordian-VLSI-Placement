//MATRIX_CPP
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <iomanip>
#include "Matrix.h"
#include "Complex.h"
using namespace std;

Matrix::Matrix(int r,int c)
{
	++MatrixCount;
	Row=r;
	Col=c;
	size=Row*Col;
	ptr=new Complex[size];
	assert(ptr!=0);
}
Matrix::Matrix(const Matrix &temp)
{
	++MatrixCount;
	Row=temp.Row;
	Col=temp.Col;
	size=Row*Col;
	ptr=new Complex[size];
	assert(ptr!=0);
	for(int i=0;i<size;i++)
		ptr[i]=temp.ptr[i];
}
Matrix::~Matrix()
{
	--MatrixCount;
	delete [] ptr;
}
int Matrix::getRow() const
{
	return Row;
}
int Matrix::getCol() const
{
	return Col;
}
int Matrix::getsize() const
{
	return size;
}
int Matrix::MatrixCount=0;
int Matrix::getMatrixCount()
{
	return MatrixCount;
}
Complex &Matrix::operator()(int i,int j)
{
	assert(0<=i-1&&i-1<Row);
	assert(0<=j-1&&j-1<Col);
	return ptr[(i-1)*Col+j-1];
}
const Matrix &Matrix::operator=(const Matrix &mat)
{
	if(&mat!=this)//检查早否自我赋值
	{
		delete [] ptr; //回收内存空间
		Row=mat.Row;
		Col=mat.Col;
		size=mat.size; //指定对象大小
		ptr=new Complex[size]; //为对象拷贝分配内存空间
		assert(ptr!=0);  //内存分配不成功时中止
		for(int i=0;i<Row;i++)
			for(int j=0;j<Col;j++)
				ptr[i*Col+j]=mat.ptr[i*Col+j]; //把矩阵元素拷贝到矩阵中
	}
	return *this;
}
Matrix Matrix::operator==(const Matrix &temp) const
{
	Matrix mat(Row,Col);
	if(Row!=temp.Row||Col!=temp.Col)
	{
		return temp;
	}
	if(Row==temp.Row&&Col==temp.Col)
	{
		for(int i=0;i<Row;i++)
			for(int j=0;j<Col;j++)
				mat.ptr[i*Col+j]=ptr[i*Col+j]==temp.ptr[i*Col+j];
	}
	return mat;
}
Matrix Matrix::operator!=(const Matrix &temp) const
{
	Matrix mat(Row,Col);
	if(Row!=temp.Row||Col!=temp.Col)
	{
		cout<<"error"<<endl;
		return temp;
	}
	if(Row==temp.Row&&Col==temp.Col)
	{		
		for(int i=0;i<Row;i++)
			for(int j=0;j<Col;j++)
				mat.ptr[i*Col+j]=ptr[i*Col+j]!=temp.ptr[i*Col+j];
	}
	return mat;
}
Matrix Matrix::operator~()//矩阵转置
{
	int i=0,j=0;
	Matrix temp(Col,Row);
	for(i=0;i<Row;i++)
		for(j=0;j<Col;j++)
			temp.ptr[j*Row+i]=ptr[i*Col+j];
	return temp;
}
Matrix Matrix::addTransfer(Matrix &mata, Matrix &matb)//add and transfer
{
	int i=0,j=0;
	int l = mata.Row;
	int r = mata.Col;
	Matrix temp(r,l);
	for(i=0;i<mata.Row;i++)
		for(j=0;j<mata.Col;j++)
			temp(j,i)=mata(i,j) + matb(i,j);
	return temp;
}
Matrix Matrix::operator+(Matrix &mat)//矩阵元素相加
{
	int i=0,j=0;
	Matrix temp(Row,Col);
	for(i=0;i<Row;i++)
		for(j=0;j<Col;j++)
			temp.ptr[i*Col+j]=ptr[i*Col+j]+mat.ptr[i*Col+j];
	return temp;
}
Matrix Matrix::operator-(Matrix &mat)//矩阵元素相减
{
	int i=0,j=0;
	Matrix temp(Row,Col);
	for(i=0;i<Row;i++)
		for(j=0;j<Col;j++)
			temp.ptr[i*Col+j]=ptr[i*Col+j]-mat.ptr[i*Col+j];
	return temp;

}
Matrix Matrix::operator*(Matrix &mat)//矩阵元素相乘
{
	int i=0,j=0;
	Matrix temp(Row,Col);
	for(i=0;i<Row;i++)
		for(j=0;j<Col;j++)
			temp.ptr[i*Col+j]=ptr[i*Col+j]*mat.ptr[i*Col+j];
	return temp;

}
Matrix Matrix::operator/(Matrix &mat)//矩阵元素相除
{
	int i=0,j=0;
	Matrix temp(Row,Col);
	for(i=0;i<Row;i++)
		for(j=0;j<Col;j++)
			temp.ptr[i*Col+j]=ptr[i*Col+j]/mat.ptr[i*Col+j];
	return temp;
}
//Matrix Matrix::operator&(Matrix &mat)//矩阵乘法
//{
//	Matrix temp(Row,mat.Col);
//	if(Col!=mat.Row)
//		cout<<"错误!A*B中,A的列数要与B的行数相同!"<<endl;
//	if(Col==mat.Row)
//	{
//		int swift = 8;
//		int edgen = Col / swift;
//		int i=0,j=0,n=0;
//		for(i=0;i<Row;i++)
//			for(j=0;j<mat.Col;j++)
//			{
//				for(n=0;n<Col-edgen;n = n+swift)
//				{
//					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j];
//					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n+1]*mat.ptr[(n+1)*mat.Col+j];
//					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n+2]*mat.ptr[(n+2)*mat.Col+j];
//					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n+3]*mat.ptr[(n+3)*mat.Col+j];
//					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n+4]*mat.ptr[(n+4)*mat.Col+j];
//					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n+5]*mat.ptr[(n+5)*mat.Col+j];
//					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n+6]*mat.ptr[(n+6)*mat.Col+j];
//					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n+7]*mat.ptr[(n+7)*mat.Col+j];
//				}
//				for(;n<Col;n++)
//				{
//					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j];
//				}
//			}
//	}
//	return temp;
//}
Matrix Matrix::operator&(Matrix &mat)//矩阵乘法
{
	Matrix temp(Row,mat.Col);
	int swift = 5;
	int swiftn = 4;
	int edgen = Col / swift;
	int mulj = mat.Col / swift;
	int edgej = mat.Col % swift;
	int edgei = Row % swift;
	if(Col!=mat.Row)
		cout<<"error!"<<endl;
	if(Col==mat.Row)
	{
		int i=0,j=0,n=0;
		for(i=0;i<(Row - edgei);i = i + swift)
#pragma region  main loop
		{
			/*for(j=0;j<(mat.Col - edgej);j = j + swift)
			{
				for(n=0;n<Col-edgen;n = n+swiftn)
				{
					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j];
					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n+1]*mat.ptr[(n+1)*mat.Col+j];
					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n+2]*mat.ptr[(n+2)*mat.Col+j];
					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n+3]*mat.ptr[(n+3)*mat.Col+j];

					temp.ptr[i*mat.Col+j+1]=temp.ptr[i*mat.Col+j+1]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j+1];
					temp.ptr[i*mat.Col+j+1]=temp.ptr[i*mat.Col+j+1]+ptr[i*Col+n+1]*mat.ptr[(n+1)*mat.Col+j+1];
					temp.ptr[i*mat.Col+j+1]=temp.ptr[i*mat.Col+j+1]+ptr[i*Col+n+2]*mat.ptr[(n+2)*mat.Col+j+1];
					temp.ptr[i*mat.Col+j+1]=temp.ptr[i*mat.Col+j+1]+ptr[i*Col+n+3]*mat.ptr[(n+3)*mat.Col+j+1];

					temp.ptr[i*mat.Col+j+2]=temp.ptr[i*mat.Col+j+2]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j+2];
					temp.ptr[i*mat.Col+j+2]=temp.ptr[i*mat.Col+j+2]+ptr[i*Col+n+1]*mat.ptr[(n+1)*mat.Col+j+2];
					temp.ptr[i*mat.Col+j+2]=temp.ptr[i*mat.Col+j+2]+ptr[i*Col+n+2]*mat.ptr[(n+2)*mat.Col+j+2];
					temp.ptr[i*mat.Col+j+2]=temp.ptr[i*mat.Col+j+2]+ptr[i*Col+n+3]*mat.ptr[(n+3)*mat.Col+j+2];

					temp.ptr[i*mat.Col+j+3]=temp.ptr[i*mat.Col+j+3]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j+3];
					temp.ptr[i*mat.Col+j+3]=temp.ptr[i*mat.Col+j+3]+ptr[i*Col+n+1]*mat.ptr[(n+1)*mat.Col+j+3];
					temp.ptr[i*mat.Col+j+3]=temp.ptr[i*mat.Col+j+3]+ptr[i*Col+n+2]*mat.ptr[(n+2)*mat.Col+j+3];
					temp.ptr[i*mat.Col+j+3]=temp.ptr[i*mat.Col+j+3]+ptr[i*Col+n+3]*mat.ptr[(n+3)*mat.Col+j+3];

					temp.ptr[i*mat.Col+j+4]=temp.ptr[i*mat.Col+j+4]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j+4];
					temp.ptr[i*mat.Col+j+4]=temp.ptr[i*mat.Col+j+4]+ptr[i*Col+n+1]*mat.ptr[(n+1)*mat.Col+j+4];
					temp.ptr[i*mat.Col+j+4]=temp.ptr[i*mat.Col+j+4]+ptr[i*Col+n+2]*mat.ptr[(n+2)*mat.Col+j+4];
					temp.ptr[i*mat.Col+j+4]=temp.ptr[i*mat.Col+j+4]+ptr[i*Col+n+3]*mat.ptr[(n+3)*mat.Col+j+4];
				}
				for(;n<Col;n++)
				{
					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j];
					temp.ptr[i*mat.Col+j+1]=temp.ptr[i*mat.Col+j+1]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j+1];
					temp.ptr[i*mat.Col+j+2]=temp.ptr[i*mat.Col+j+2]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j+2];
					temp.ptr[i*mat.Col+j+3]=temp.ptr[i*mat.Col+j+3]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j+3];
					temp.ptr[i*mat.Col+j+4]=temp.ptr[i*mat.Col+j+4]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j+4];
				}
			}
			for(;j<mat.Col;j++)
			{
				for(n=0;n<Col;n++)
				{
					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j];
				}
			}*/
			for(j=0;j<(mat.Col - edgej);j = j + swift)
				for(n=0;n<Col;n++)
				{
					temp.ptr[(i)*mat.Col+j]=temp.ptr[(i)*mat.Col+j]+ptr[(i)*Col+n]*mat.ptr[n*mat.Col+j];
					temp.ptr[(i)*mat.Col+j+1]=temp.ptr[(i)*mat.Col+j+1]+ptr[(i)*Col+n]*mat.ptr[n*mat.Col+j+1];
					temp.ptr[(i)*mat.Col+j+2]=temp.ptr[(i)*mat.Col+j+2]+ptr[(i)*Col+n]*mat.ptr[n*mat.Col+j+2];
					temp.ptr[(i)*mat.Col+j+3]=temp.ptr[(i)*mat.Col+j+3]+ptr[(i)*Col+n]*mat.ptr[n*mat.Col+j+3];
					temp.ptr[(i)*mat.Col+j+4]=temp.ptr[(i)*mat.Col+j+4]+ptr[(i)*Col+n]*mat.ptr[n*mat.Col+j+4];
				}
			for(;j<mat.Col;j++)
				for(n=0;n<Col;n++)
				{
					temp.ptr[(i)*mat.Col+j]=temp.ptr[(i)*mat.Col+j]+ptr[(i)*Col+n]*mat.ptr[n*mat.Col+j];
				}

			for(j=0;j<(mat.Col - edgej);j = j + swift)
				for(n=0;n<Col;n++)
				{
					temp.ptr[(i+1)*mat.Col+j]=temp.ptr[(i+1)*mat.Col+j]+ptr[(i+1)*Col+n]*mat.ptr[n*mat.Col+j];
					temp.ptr[(i+1)*mat.Col+j+1]=temp.ptr[(i+1)*mat.Col+j+1]+ptr[(i+1)*Col+n]*mat.ptr[n*mat.Col+j+1];
					temp.ptr[(i+1)*mat.Col+j+2]=temp.ptr[(i+1)*mat.Col+j+2]+ptr[(i+1)*Col+n]*mat.ptr[n*mat.Col+j+2];
					temp.ptr[(i+1)*mat.Col+j+3]=temp.ptr[(i+1)*mat.Col+j+3]+ptr[(i+1)*Col+n]*mat.ptr[n*mat.Col+j+3];
					temp.ptr[(i+1)*mat.Col+j+4]=temp.ptr[(i+1)*mat.Col+j+4]+ptr[(i+1)*Col+n]*mat.ptr[n*mat.Col+j+4];
				}
			for(;j<mat.Col;j++)
				for(n=0;n<Col;n++)
				{
					temp.ptr[(i+1)*mat.Col+j]=temp.ptr[(i+1)*mat.Col+j]+ptr[(i+1)*Col+n]*mat.ptr[n*mat.Col+j];
				}
				
			for(j=0;j<(mat.Col - edgej);j = j + swift)
				for(n=0;n<Col;n++)
				{
					temp.ptr[(i+2)*mat.Col+j]=temp.ptr[(i+2)*mat.Col+j]+ptr[(i+2)*Col+n]*mat.ptr[n*mat.Col+j];
					temp.ptr[(i+2)*mat.Col+j+1]=temp.ptr[(i+2)*mat.Col+j+1]+ptr[(i+2)*Col+n]*mat.ptr[n*mat.Col+j+1];
					temp.ptr[(i+2)*mat.Col+j+2]=temp.ptr[(i+2)*mat.Col+j+2]+ptr[(i+2)*Col+n]*mat.ptr[n*mat.Col+j+2];
					temp.ptr[(i+2)*mat.Col+j+3]=temp.ptr[(i+2)*mat.Col+j+3]+ptr[(i+2)*Col+n]*mat.ptr[n*mat.Col+j+3];
					temp.ptr[(i+2)*mat.Col+j+4]=temp.ptr[(i+2)*mat.Col+j+4]+ptr[(i+2)*Col+n]*mat.ptr[n*mat.Col+j+4];
				}
			for(;j<mat.Col;j++)
				for(n=0;n<Col;n++)
				{
					temp.ptr[(i+2)*mat.Col+j]=temp.ptr[(i+2)*mat.Col+j]+ptr[(i+2)*Col+n]*mat.ptr[n*mat.Col+j];
				}
			
			for(j=0;j<(mat.Col - edgej);j = j + swift)
				for(n=0;n<Col;n++)
				{
					temp.ptr[(i+3)*mat.Col+j]=temp.ptr[(i+3)*mat.Col+j]+ptr[(i+3)*Col+n]*mat.ptr[n*mat.Col+j];
					temp.ptr[(i+3)*mat.Col+j+1]=temp.ptr[(i+3)*mat.Col+j+1]+ptr[(i+3)*Col+n]*mat.ptr[n*mat.Col+j+1];
					temp.ptr[(i+3)*mat.Col+j+2]=temp.ptr[(i+3)*mat.Col+j+2]+ptr[(i+3)*Col+n]*mat.ptr[n*mat.Col+j+2];
					temp.ptr[(i+3)*mat.Col+j+3]=temp.ptr[(i+3)*mat.Col+j+3]+ptr[(i+3)*Col+n]*mat.ptr[n*mat.Col+j+3];
					temp.ptr[(i+3)*mat.Col+j+4]=temp.ptr[(i+3)*mat.Col+j+4]+ptr[(i+3)*Col+n]*mat.ptr[n*mat.Col+j+4];
				}
			for(;j<mat.Col;j++)
				for(n=0;n<Col;n++)
				{
					temp.ptr[(i+3)*mat.Col+j]=temp.ptr[(i+3)*mat.Col+j]+ptr[(i+3)*Col+n]*mat.ptr[n*mat.Col+j];
				}
				
			for(j=0;j<(mat.Col - edgej);j = j + swift)
				for(n=0;n<Col;n++)
				{
					temp.ptr[(i+4)*mat.Col+j]=temp.ptr[(i+4)*mat.Col+j]+ptr[(i+4)*Col+n]*mat.ptr[n*mat.Col+j];
					temp.ptr[(i+4)*mat.Col+j+1]=temp.ptr[(i+4)*mat.Col+j+1]+ptr[(i+4)*Col+n]*mat.ptr[n*mat.Col+j+1];
					temp.ptr[(i+4)*mat.Col+j+2]=temp.ptr[(i+4)*mat.Col+j+2]+ptr[(i+4)*Col+n]*mat.ptr[n*mat.Col+j+2];
					temp.ptr[(i+4)*mat.Col+j+3]=temp.ptr[(i+4)*mat.Col+j+3]+ptr[(i+4)*Col+n]*mat.ptr[n*mat.Col+j+3];
					temp.ptr[(i+4)*mat.Col+j+4]=temp.ptr[(i+4)*mat.Col+j+4]+ptr[(i+4)*Col+n]*mat.ptr[n*mat.Col+j+4];
				}
			for(;j<mat.Col;j++)
				for(n=0;n<Col;n++)
				{
					temp.ptr[(i+4)*mat.Col+j]=temp.ptr[(i+4)*mat.Col+j]+ptr[(i+4)*Col+n]*mat.ptr[n*mat.Col+j];
				}
		}
#pragma endregion
		for(;i<Row; i++)
		{
			for(j=0;j<(mat.Col - edgej);j = j + swift)
				for(n=0;n<Col;n++)
				{
					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j];
					temp.ptr[i*mat.Col+j+1]=temp.ptr[i*mat.Col+j+1]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j+1];
					temp.ptr[i*mat.Col+j+2]=temp.ptr[i*mat.Col+j+2]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j+2];
					temp.ptr[i*mat.Col+j+3]=temp.ptr[i*mat.Col+j+3]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j+3];
					temp.ptr[i*mat.Col+j+4]=temp.ptr[i*mat.Col+j+4]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j+4];
				}
			for(;j<mat.Col;j++)
				for(n=0;n<Col;n++)
				{
					temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j];
				}
		}
	}
	return temp;
}
Matrix Matrix::operator|(Matrix &mat)//负矩阵乘法
{
	Matrix temp(Row,mat.Col);
	if(Col!=mat.Row)
		cout<<"error"<<endl;
	if(Col==mat.Row)
	{
		int i=0,j=0,n=0;
		for(i=0;i<Row;i++)
			for(j=0;j<mat.Col;j++)
				for(n=0;n<Col;n++)
				temp.ptr[i*mat.Col+j]=(temp.ptr[i*mat.Col+j]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j]) * (-1);							
	}
	return temp;
}
Matrix Matrix::reverse(Matrix &mat)
{
	Matrix temp = mat;
	Matrix Identity(mat.Row, mat.Col);
	Identity = addIdentity(Identity, 1, 1, mat.Row);
	temp = Identity / temp;
	return temp;
}
Matrix Matrix::amplify(Matrix &mat, int pa)
{
	Matrix temp = mat;
	int i=0,j=0,n=0;
	for(i=0;i<mat.Row;i++)
		for(j=0;j<mat.Col;j++)
			temp(i+1,j+1) = temp(i+1,j+1) * pa;
				//temp.ptr[i*mat.Col+j]=temp.ptr[i*mat.Col+j]+ptr[i*Col+n]*mat.ptr[n*mat.Col+j];							
	return temp;
}
Matrix Matrix::addIdentity(Matrix &mat, int sx, int sy, int rw)//add an Identity matrix
{
	Matrix temp = mat;
	int i=sx,j=sy;
	int n=1;
	for(;n<=rw;i++,j++,n++)
		temp(i,j) = 1;							
	return temp;
}
Matrix Matrix::getBlock(Matrix &matp, Matrix &matb, int sx, int sy, int rw, int cl)
{
	Matrix temp = matb;
	int pi = sx, pj = sy;
	int bi = 1, bj = 1;
	for(bj = 1, pj = sy; pj<=cl + sy - 1; bj++, pj++)
		for(bi = 1, pi = sx ;pi<=rw + sx - 1; bi++, pi++)
			temp(bi,bj) = matp(pi,pj);
	return temp;
}
Matrix Matrix::addBlock(Matrix &matp, Matrix &matb, int sx, int sy, int rw, int cl)
{
	Matrix temp = matp;
	int pi = sx, pj = sy;
	int bi = 1, bj = 1;
	for(bj = 1, pj = sy; pj<=cl + sy - 1; bj++, pj++)
		for(bi = 1, pi = sx;pi<=rw + sx - 1; bi++, pi++)
			temp(pi,pj) = matb(bi,pj);
	return temp;
}
ostream &operator<<(ostream &output,const Matrix &mat)
{
	int i=0,j=0;
	for(i=0;i<mat.Row;i++)
	{
		for(j=0;j<mat.Col;j++)
			output<<setprecision(3)<<setw(5)<<mat.ptr[i*mat.Col+j]<<'\t'<<setw(5);
		output<<endl;
	}
	return output;
}
istream &operator>>(istream &input,Matrix &mat)
{
	int i=0,j=0;
	for(i=0;i<mat.Row;i++)
		for(j=0;j<mat.Col;j++)
			input>>mat.ptr[i*mat.Col+j];
	return input;
}
Matrix diag(Matrix &mat)
{
	int i=0;
	Matrix temp(mat.Row,1);
	for(i=0;i<mat.Row;i++)
		temp.ptr[i]=mat.ptr[i*mat.Col+i];
	return temp;
}
Matrix con(Matrix &mat)
{
	int i=0,j=0;
	Matrix temp(mat.Row,mat.Col);
	for(i=0;i<mat.Row;i++)
		for(j=0;j<mat.Col;j++)
			temp.ptr[i*temp.Col+j]=mat.ptr[i*mat.Col+j].con();
	return temp;
}
Matrix Matrix::conversionMatrix(Matrix &C, int index[])
{
	//int big = 7;
	//int small = 2;
	int checkindex[95];

	for(int i=1; i<=94;i++)
		checkindex[i] = index[i];

	//double testC[7][7];
	//for(int i=0; i<7;i++)
	//	for(int j=0; j<7; j++)
	//			testC[i][j] = C(i+1, j+1);

	Matrix temp1 = C;
	Matrix temp2 = C;
	int i,j;
	for(i = 1; i<=C.Row; i++)// do mapping for the rows
	{
		for(j = 1; j<=C.Col; j++)
		{
			temp1(i, j) = C(index[i], j);// moving the rows	
		}
	}
	//double testC1[7][7];
	//for(int i=0; i<7;i++)
	//	for(int j=0; j<7; j++)
	//			testC1[i][j] = temp1(i+1, j+1);

	for(i = 1; i<=C.Col; i++)// do mapping
	{
		for(j = 1; j<=C.Row; j++)
		{
			temp2(j, i) = temp1(j, index[i]);// moving the rows	
		}
	}

	//double testC2[7][7];
	//for(int i=0; i<7;i++)
	//	for(int j=0; j<7; j++)
	//			testC2[i][j] = temp2(i+1, j+1);

	return temp2;
}
Matrix Matrix::conversionVector(Matrix &C, int index[])
{
	Matrix temp = C;
	int i;
	int j=1;
	//double testC[7];
	//for(int i=0; i<7;i++)
	//	testC[i] = C(i+1, 1);

	for(i = 1; i<=C.Row; i++)// do mapping
	{
		temp(i, j) = C(index[i], j);// moving the rows	

	}
	//double testC2[7];
	//for(int i=0; i<7;i++)
	//	testC2[i] = temp(i+1, 1);
	return temp;
}
Matrix Matrix::conversionVectorback(Matrix &C, int index[])
{
	Matrix temp = C;
	int i;
	int j=1;
	//double testC[7];
	//for(int i=0; i<7;i++)
	//	testC[i] = C(i+1, 1);

	for(i = 1; i<=C.Row; i++)// do mapping
	{
		temp(index[i], j) = C(i, j);// moving the rows	

	}
	//double testC2[7];
	//for(int i=0; i<7;i++)
	//	testC2[i] = temp(i+1, 1);
	return temp;
}
Matrix Matrix::reverseGauss(Matrix &A)
{
    int i, j, k;
	int n=A.Row;
    double max, temp;
    //将A矩阵存放在临时矩阵t[n][n]中
	Matrix t = A;
	Matrix B = A;
    //初始化B矩阵为单位阵
    for (i = 1; i <= n; i++)        
    {
        for (j = 1; j <= n; j++)
        {
            B(i,j) = (i == j) ? (float)1 : 0;
        }
    }
    for (i = 1; i <= n; i++)
    {
        //寻找主元
        max = t(i, i);
        k = i;
        for (j = i+1; j <= n; j++)
        {
            if (fabs(t(j,i)) > fabs(max))
            {
                max = t(j,i);
                k = j;
            }
        }
        //如果主元所在行不是第i行，进行行交换
        if (k != i)
        {
            for (j = 1; j <= n; j++)
            {
                temp = t(i,j);
                t(i,j) = t(k,j);
                t(k,j) = temp;
                //B伴随交换
                temp = B(i,j);
                B(i,j) = B(k,j);
                B(k,j) = temp;
            }
        }
        //判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵
        if (!t(i,i))
        {
            cout << "There is no inverse matrix!";
            return false;
        }
        //消去A的第i列除去i行以外的各行元素
        temp = t(i,i);
        for (j = 1; j <= n; j++)
        {
            t(i,j) = t(i,j) / temp;        //主对角线上的元素变为1
            B(i,j) = B(i,j) / temp;        //伴随计算
        }
        for (j = 1; j <= n; j++)        //第0行->第n行
        {
            if (j != i)                //不是第i行
            {
                temp = t(j,i);
				int swift = 4;
				int edge = n % 4;
				for (k = 1; k <= n - edge; k = k + swift)        //第j行元素 - i行元素*j列i行元素
                {
                    t(j,k) = t(j,k) - t(i,k)*temp;
                    B(j,k) = B(j,k) - B(i,k)*temp;

					t(j,k+1) = t(j,k+1) - t(i,k+1)*temp;
                    B(j,k+1) = B(j,k+1) - B(i,k+1)*temp;

					t(j,k+2) = t(j,k+2) - t(i,k+2)*temp;
                    B(j,k+2) = B(j,k+2) - B(i,k+2)*temp;

					t(j,k+3) = t(j,k+3) - t(i,k+3)*temp;
                    B(j,k+3) = B(j,k+3) - B(i,k+3)*temp;
                }
				for (; k <= n; k++)       
                {
                    t(j,k) = t(j,k) - t(i,k)*temp;
                    B(j,k) = B(j,k) - B(i,k)*temp;
				}
            }
        }
    }
    return B;
}