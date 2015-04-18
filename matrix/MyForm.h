#pragma once

#include <iostream>
#include <windows.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <vcclr.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <vector>
#include "Matrix.h"

#define cellMax 2000// the maximum number of modules
#define netMax 4000// the maximum number of the net
#define pinMax 32// the maximum number of pins in one module
#define formW 1000
#define formH 700
#define rootW 700
#define rootH 700
#define buttonM (formW - rootW -2)/3
#define buttonH 50
#define textW formW - rootW -1
#define textH formH - buttonH -2
#define beyond 1000
#define tch  7
#define tq1 1
#define tq  2

namespace GORDIAN {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace std;
	
	struct module// including the module information, the nets should be a link list
	{
		int moduleNum;// the module name
		float moduleW;// width 
		float moduleH;// height
		float moduleX;// the x position of the module
		float moduleY;// the y position of the module
		int netSum;// total number of the nets it is connected to, it's the valid length of he netNum!!!!!! same to the pinNum
		int weight[pinMax];// weight
		int netNum[pinMax];// the net the pins belong to, they are all correspondingly
		float pinX[pinMax];// the x position of the pin
		float pinY[pinMax];// the y position of the pin
		int pinNum;// the total number of the pins
		int mapNum;// mapping the movable modules
		float area;// the area of the module occupis
		char MorF;// movable or fixed
		module *next;// link to another net module
	};

	struct partition// a tree for partition, always use the leaf node
	{
		char HorV;
		int num;
		float sx;
		float sy;
		float ex;
		float ey;
		bool used;
		float aveX;
		float aveY;
		partition *left;
		partition *right;
	};

	class region
	{
	public:
		module *root;
		module *netArray[netMax];
		partition *divide;
		region(void);
		module *createModule(module *, int, int, int, int, int, float, float, int, int, char);
		module *createNetArray(module *, module *[]);
		module *insertNet(module *, module *, int);
		module *mapMovable(module *);
		void readNetArray(module *[], int);
		int checkIfUse(module *, int, int, int, int, int);
		void addOnePin(module *, int, int, int, int);
		module *updateModule(module *, Matrix &, Matrix &, int) ;
		module *updateNetArray(module *, module *[]);
		module *updatePosition(module *, module *, int);
		partition *insertArea(float, float, float, float);
		Matrix &findLeaf(partition *, Matrix &);

		Matrix C;// all initialized to 0
		Matrix dx;
		Matrix dy;
		Matrix edge;
		int row;
		int realQval;

	private:
		bool addTF;
		//MatrixXf m;
	};
	/// <summary>
	/// MyForm 摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void);

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	/*********************************************************************************/
#pragma region system variables

		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
	private: System::ComponentModel::Container^ components;
	private: System::Windows::Forms::Button^  button1;// GORDIAN
	private: System::Windows::Forms::Button^  button2;// get net list	
	private: System::Windows::Forms::Button^  button3;// link wire
	private: System::Windows::Forms::TextBox^  textBox1; 
	public: int Num_Cells;// total number of the cells
	public: int Num_Mcells;// total number of the movabel cells
	public: int Num_Fcells;// total number of the fixed cells
	public: int Num_Nets;// total number of the nets
	public: int W;// width of the root region
	public: int H;// height of the root region
	public: int ratio;// amplify the region by a constant ratio
	public: int moduleNum;// the number of the module
	public: int netNum;// the number of the net
	public: int weight;// weight, default value is 1
	public: int moduleW;// width 
	public: int moduleH;// height
	public: float moduleX;// the x position of the module
	public: float moduleY;// the y position of the module
	public: int pinX;// the x position of the pin
	public: int pinY;// the y position of the pin
	public: char MorF;// movable or fixed
	public: int level;// create C only in level 0
	public: int regionNum;//
	public: float startx;
	public: float starty;
	public: float endx; 
	public: float endy; 
	public: float overbound;
	public: float totalLengh;
	public: int qval;
	//public: int fix;
	//public: int fixArea;

#pragma endregion

#pragma region Windows Form System function	

	private: System::Void txtRead(System::String^ filename) ; 
	private: System::Void cleanRoot(void) ;//  clean the root area
	public: System::Void DrawRectangleRectangle(float, float, float, float);//System::Drawing::Rectangle^ rect);
	private: System::Void DrawLineLine(float, float, float, float);
	private: System::Void MarshalString ( String ^ s, string& os ); 
	private: void drawNew(module *);
	private: void drawModule(module *temp);
	private: void drawNetArray(module *[]);
	private: void drawWire(module *[]);
	private: void drawPartition(partition *);
	private: void setupObjectiveFunction(module *[], module *, int, int, int, int,
				 float, float, float, float);// key function, promped by the link list of nets
	private: void partitionArea(partition *, module *, int);
	private: void calculatePosition(partition *, module *[], module *, int, int, int, int,
				 float, float, float, float);
	private: int contPin(module *);
	private: char checkLine(string);
	private: int getNum(string);
	private: int getNetNum(string);
	private: int getWeight(string);
	private: char getModuleType(string);
	private: int getModuleInfo(string, char, int, char, int);
	private: template <class Type> Type stringToNum(const string& str);

#pragma endregion

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
	private: System::Void InitializeComponent(void);
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) ;
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) ;
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) ;
#pragma endregion

	};
}
