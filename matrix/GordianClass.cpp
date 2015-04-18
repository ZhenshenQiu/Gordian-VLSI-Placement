
#include "MyForm.h"

#define FILENAME "netlist_10%5b1%5d.txt"
//#define FILENAME "netlist_30%5b1%5d_class.txt"
//#define FILENAME "netlist_40%5b1%5d.txt"
//#define FILENAME "boss.txt"
//#define FILENAME "netlist.txt"
//#define FILENAME "kid.txt"
using namespace GORDIAN;


//
//
//global variables
region myRegion;

MyForm::MyForm()
{	
	srand((int)time(0));
	level = 0;
	regionNum = 0;
	float startx = 0;
	float starty = 0;
	float endx = 0; 
	float endy = 0; 
	qval = 1;//one region originally
	overbound = 0;
	totalLengh = 0;
	InitializeComponent();
} 

region::region()
{
	root = NULL;
	divide = NULL;
	row = 1;
	addTF = false;// no pin added
}

void MyForm::InitializeComponent()
{
	this->button2 = (gcnew System::Windows::Forms::Button());
	this->textBox1 = gcnew TextBox;
	// 
	// button2, get list
	// 
	this->button2->Location = System::Drawing::Point(rootW + 2, formH - buttonH);
	this->button2->Name = L"ReadList";
	this->button2->Size = System::Drawing::Size((formW - rootW -2)/3, buttonH);
	this->button2->TabIndex = 0;
	this->button2->Text = L"ReadList";
	this->button2->UseVisualStyleBackColor = true;
	this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
	//
	// textbox1
	//
	this->textBox1->Location = System::Drawing::Point(rootW + 1, 0);
	this->textBox1->Size = System::Drawing::Size(textW, textH);
    this->textBox1->Multiline = true;// Set the Multiline property to true.
    this->textBox1->ScrollBars = ScrollBars::Vertical;// Add vertical scroll bars to the TextBox control.      
    this->textBox1->AcceptsReturn = true;// Allow the RETURN key to be entered in the TextBox control.      
    this->textBox1->AcceptsTab = true;// Allow the TAB key to be entered in the TextBox control.
    this->textBox1->WordWrap = true;// Set WordWrap to True to allow text to wrap to the next line.
    this->textBox1->Text = "Welcome!";// Set the default text of the control.
	// 
	// MyForm
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(formW, formH + 1);
	this->Controls->Add(this->button2);
	this->Controls->Add(this->textBox1);
	this->Name = L"MyForm";
	this->Text = L"MyForm";
	this->ResumeLayout(false);
	
}	

Void MyForm::button3_Click(System::Object^  sender, System::EventArgs^  e) 
{
	drawWire(myRegion.netArray);
	String ^str;
	str = "The total length of the wires:  " + System::Convert::ToString(static_cast<double>(totalLengh/ratio));
	MessageBox::Show(this,str);
 }

Void MyForm::button1_Click(System::Object^  sender, System::EventArgs^  e) 
{
	//for(int i=0; i<15; i++)
	{
	level++;// level == 1 means level 0
	totalLengh = 0;// reset the total length
	setupObjectiveFunction(myRegion.netArray, myRegion.root, Num_Mcells, Num_Mcells, Num_Nets, level, startx, starty, startx + endx, startx + endy);
	calculatePosition(myRegion.divide, myRegion.netArray, myRegion.root, Num_Mcells, Num_Mcells, Num_Nets, level, startx, starty, startx + endx, startx + endy);
	cleanRoot();
	drawPartition(myRegion.divide);
	drawModule(myRegion.root);
	qval = 0;//reset the area number
	partitionArea(myRegion.divide, myRegion.root, level);
	}
 }

/*****************************  set   ***************************/
void MyForm::setupObjectiveFunction(module *net[], module *temp, const int cw, const int ch, const int nn, 
									const int lvl, const float sx, const float sy, const float ex, const float ey)
{
	if(lvl == 1)// do only in level 0
	{
#pragma region pseudo code implement

	Matrix Ctemp(cw,ch);// all initialized to 0
	Matrix dxtemp(cw,1);
	Matrix dytemp(cw,1);
	// for v belongs to N
	//
	// loop 1 
	for(int i=1; i<=nn; i++)//read the nets, each net is linked to a series of modules which are included in this net
	{
		int Mv = contPin(net[i]);
		float e = (net[i]->weight[0]) * 2 / static_cast<float>(Mv);//(Num_Mcells) ;//(Mv);
		#pragma region
		// forAllMovableModule(int);
		module *searchMovable = net[i];
		module *searchOther = net[i];
		double lookdx=0;
		double lookdy=0;
		int lookmap = 0;
		int lookothermap =0;
		//
		//
		//loop 2
		while( searchMovable != NULL )// search all nodules in this net
		{
			if( searchMovable->MorF == 'M' )// only for movable modules
			{
				lookmap = searchMovable->mapNum;
				Ctemp(searchMovable->mapNum  , searchMovable->mapNum ) = Ctemp(searchMovable->mapNum  , searchMovable->mapNum ) + (e * (Mv -1));// Cuu
				dxtemp(searchMovable->mapNum  , 1) = dxtemp(searchMovable->mapNum  , 1) + (searchMovable->pinX[0]) * e * (Mv -1);// dxu
				dytemp(searchMovable->mapNum  , 1) = dytemp(searchMovable->mapNum  , 1) + (searchMovable->pinY[0]) * e * (Mv -1);// dyu
				//
				//
				//loop 3
				#pragma region
				while( searchOther != NULL)// for lambda belongs to Mv\{u}
				{
					lookothermap = searchMovable->mapNum;
					if( searchOther->moduleNum != searchMovable->moduleNum )
					{
						if( searchOther->MorF == 'M' )// for lambda belongs to Movable modules
						{
							Ctemp(searchMovable->mapNum , searchOther->mapNum) = Ctemp(searchMovable->mapNum , searchOther->mapNum) - e;// Cul
							dxtemp(searchMovable->mapNum , 1) = dxtemp(searchMovable->mapNum , 1) - (searchOther->pinX[0]) * e;// dxu
							dytemp(searchMovable->mapNum , 1) = dytemp(searchMovable->mapNum , 1) - (searchOther->pinY[0]) * e;// dxu
						}
						else// for lambda belongs to Fixed modules
						{
							dxtemp(searchMovable->mapNum, 1) = dxtemp(searchMovable->mapNum, 1) - e * ( (searchOther->moduleX) + (searchOther->pinX[0]) );// dxu
							dytemp(searchMovable->mapNum, 1) = dytemp(searchMovable->mapNum, 1) - e * ( (searchOther->moduleY) + (searchOther->pinY[0]) );// dxu
						}
					}
					searchOther = searchOther->next;
				}
				
				#pragma endregion
				
				searchOther = net[i];// reset the searchOther pointer
			}
			searchMovable = searchMovable->next;
		}
		#pragma endregion
	}

	myRegion.C = Ctemp;
	myRegion.dx = dxtemp;
	myRegion.dy = dytemp;


	// the C matrix is symmetric
	
	//MessageBox::Show(this,"Congratulations! Now we get: C, dx, dy !");
	
#pragma endregion
	}
}

#pragma region partition
partition *region::insertArea(float sx, float sy, float ex, float ey )
{
	partition *temp = new partition;
	temp->sx = sx;
	temp->sy = sy;
	temp->ex = ex;
	temp->ey = ey;
	temp->used = false;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

Matrix &region::findLeaf( partition *div, Matrix &mat)
{	
	if(div->left == NULL && div->right == NULL )// it's a leaf node
	{
		if( div->used == false)
		{
			mat(row,1) = div->sx;
			mat(row,2) = div->sy;
			mat(row,3) = div->ex;
			mat(row,4) = div->ey;
			row++;
			return mat;
		}
		else// this region has no modules
		{
			realQval--;
			return mat;// do not increase the row
		}
	}
	else
	{
		findLeaf(div->left, mat);
		findLeaf(div->right, mat);
		return mat;
	}
}
void MyForm::partitionArea( partition *div, module *temp, const int lvl)
{	
	if(div->left == NULL && div->right == NULL)// for the region to divide
	{
		int moduleNumArea = 0;// calculate the total number of modules in this area
		float totalx = 0;
		float totaly = 0;
		module *w = temp;
		float sxcheck = div->sx;
		float sycheck = div->sy;
		float excheck = div->ex;
		float eycheck = div->ey;
		if(sxcheck == startx)
		{
			sxcheck = startx - beyond;
		}
		if(sycheck == starty)
		{
			sycheck = starty - beyond;
		}
		if(excheck == endx)
		{
			excheck = endx - beyond;
		}
		if(eycheck ==endy)
		{
			eycheck = endy - beyond;
		}
		while( w != NULL)
		{
			if( ( w->moduleX >= sxcheck ) && (w->moduleY >= sycheck  )
					&& ( w->moduleX < excheck ) && ( w->moduleY < eycheck  ))
			{
				if( w->MorF == 'M' )
				{
				
					moduleNumArea ++;
					totalx += w->moduleX;
					totaly += w->moduleY;
				}
			}
			w = w->next;
		}
		
		if(moduleNumArea == 1)// do not partition anymore
		{
			qval++;
			div->used = false;// this region is used
		}
		else if(moduleNumArea > 1)
		{
			qval += 2;
			div->used = false;// this region is used
			//qval = static_cast<int>(pow(2,level));// calculate the partition area of next partition
			div->aveX = totalx / moduleNumArea;
			div->aveY = totaly / moduleNumArea;
			int a =0 ;
			if( (div->ey - div->sy) >= (div->ex - div->sx)) 
			{
				div->HorV = 'H';
				div->left = myRegion.insertArea(div->sx, div->sy, div->ex, div->aveY);
				div->right = myRegion.insertArea(div->sx, div->aveY, div->ex, div->ey);
			}
			else// x >= y
			{
				div->HorV = 'V';
				div->left = myRegion.insertArea(div->sx, div->sy, div->aveX, div->ey);
				div->right = myRegion.insertArea(div->aveX, div->sy, div->ex, div->ey);
			}
		}
		else// module == 0
		{
			div->used = true;// this region is not used
			//area number do not increase, mark the node and it won't be used in the later calculating
			//MessageBox::Show(this,"Gorgian ends!");
		}
		return;
	}
	else
	{
		partitionArea(div->left, temp, lvl);
		partitionArea(div->right, temp, lvl);
		return;
	}
}

void MyForm::drawPartition(partition *div)
{
	if(div == NULL)
	{
		return;
	}
	else
	{
		if(div->HorV == 'H')
			DrawLineLine(div->sx, div->aveY, div->ex, div->aveY);
		else
			DrawLineLine(div->aveX, div->sy, div->aveX, div->ey);
		drawPartition(div->left);
		drawPartition(div->right);
		return;
	}
}


void MyForm::calculatePosition(partition *div, module *net[], module *temp, const int cw, const int ch, const int nn, 
									const int lvl, const float sx, const float sy, const float ex, const float ey)
{

#pragma endregion


#pragma region calculate the A	, ux  , uy
	
		//find leaf regions
	partition *divtemp = div;
	myRegion.realQval = qval;
	Matrix leafArea(qval, 4); 
	myRegion.row =1;//reset
	leafArea = myRegion.findLeaf(divtemp,leafArea);

	//qval = myRegion.realQval;

	Matrix A(qval,ch);// A = [ D | B ]
	Matrix D(qval,qval);
	Matrix B(qval,ch - qval);
	Matrix lc(ch - qval, 1);// little c
	Matrix Z(ch,ch - qval);
	Matrix ux(qval, 1);
	Matrix uy(qval, 1);
	Matrix cx(ch - qval, 1);
	Matrix cy(ch - qval, 1);
	Matrix ZCZ(ch - qval, ch - qval);
	Matrix xi(ch - qval, 1);
	Matrix yi(ch - qval, 1);
	Matrix x0(ch, 1);
	Matrix xd(qval, 1);
	Matrix x(ch, 1);// solution of the modules' X positions
	Matrix y0(ch, 1);
	Matrix yd(qval, 1);
	Matrix y(ch, 1);// solution of the modules' Y positions
	Matrix sequence(1,Num_Mcells);//mapping the C and A
	module *w = temp;
	int i;
	int startRow = 1;// the start row number
	int startCol = 1 + qval;// the start column number	
	int dp = 1;//
	for(i = 0; i < qval; i++)// for each partitioned area
	{
		double sxtmp = leafArea(i+1, 1);
		double sytmp = leafArea(i+1, 2);
		double extmp = leafArea(i+1, 3);
		double eytmp = leafArea(i+1, 4);
		double sxcheck = sxtmp;
		double sycheck = sytmp;
		double excheck = extmp;
		double eycheck = eytmp;
		if(sxtmp == sx)
		{
			sxcheck = sxtmp - beyond;
		}
		if(sytmp == sy)
		{
			sycheck = sytmp - beyond;
		}
		if(extmp == ex)
		{
			excheck = extmp + beyond;
		}
		if(eytmp == ey)
		{
			eycheck = eytmp + beyond;
		}
		int maxModule = 0;// initialize the max module name, default one is the first one
		float areaSum = 0;// calculate the total area of the modules in this area
		float areaMax = 0;// record the max area in these modules, default one is the first one
		int moduleNumArea = 0;// calculate the total number of modules in this area to be the start position of the next row
		while( w != NULL)
		{
			if( w->MorF == 'M' )
			{
				if( ( w->moduleX >= sxcheck ) && (w->moduleY >= sycheck  )
					&& ( w->moduleX < excheck ) && ( w->moduleY < eycheck  ))
				{
					areaSum += w->area;
					moduleNumArea ++;
					if(w->area > areaMax )// find a bigger one
					{
						areaMax = w->area;
						maxModule = w->moduleNum;// record the max module
					}
				}
			}
			w = w->next;
		}
		w = temp;// return the w pointer to the root
		//
		//
		// initialize the u
		ux(i+1,1) = sxtmp + (extmp - sxtmp) / 2;
		uy(i+1,1) = sytmp + (eytmp - sytmp) / 2;

		//
		//
		//
		int j;
		for(j = startCol; w != NULL; )// search all the movable modules
		{
			if( w->MorF == 'M' && ( w->moduleX >= sxcheck  ) && ( w->moduleY >= sycheck )
				&& ( w->moduleX < excheck   ) && ( w->moduleY < eycheck ) )
			{
				if(w->moduleNum == maxModule)// it is the biggest one
				{
					A(startRow, dp) = w->area / areaSum;
					sequence(1,dp) = w->mapNum;
					dp++;
				}
				else
				{
					A(startRow, j) = w->area / areaSum;
					sequence(1,j) = w->mapNum;
					j++;
				}
			}
			w = w->next;
		}
		w = temp;// return the w pointer to the root
		startRow++;
		startCol = j;
	}
	

	//
	//
	//reshape A
	//
	int index[cellMax+1];
	for(i=1;i<=Num_Mcells;i++)
		index[i] = static_cast<int>(sequence(1,i));

#pragma endregion
		//should do reshape 
		//for(int i=0; i<
		//
	Matrix Cnew = myRegion.C.conversionMatrix(myRegion.C, index);// all initialized to 0
	Matrix dxnew = myRegion.dx.conversionVector(myRegion.dx, index);
	Matrix dynew = myRegion.dy.conversionVector(myRegion.dy, index);

#pragma region  calculate new x & y

		D = D.getBlock(A, D, 1, 1, qval, qval);
		B = B.getBlock(A, B, 1, qval+1, qval, ch-qval);
		Matrix DR(qval, qval);
		DR = D.reverseGauss(D);
		Matrix DBR(qval, ch-qval);
		DBR = (DR) & B;// '|' means '-&'
		DBR = DBR.amplify(DBR, -1);//DBR = -D-1B
		//
		Z = Z.addBlock(Z, DBR, 1, 1, qval, ch-qval);
		Z = Z.addIdentity(Z, qval+1, 1, ch - qval);
		
		Matrix ZCZ0 = ~Z;
		Matrix ZCZ1 = ZCZ0 & Cnew;
		ZCZ = ZCZ1 & Z;

		x0 = x0.addBlock( x0, ((DR) & ux) , 1, 1, qval, 1);
		y0 = y0.addBlock( y0, ((DR) & uy) , 1, 1, qval, 1);
		

		Matrix Cx0 = Cnew & x0;
		Matrix Cy0 = Cnew & y0;

		Matrix Cx0dx = ~(Cx0 + dxnew);
		Matrix Cy0dy = ~(Cy0 + dynew);

		cx = ~(Cx0dx & Z);
		cy = ~(Cy0dy & Z);
		
		Matrix ZCZR = ZCZ.reverseGauss(ZCZ);

		xi = (ZCZR) & (cx.amplify(cx,-1));
		yi = (ZCZR) & (cy.amplify(cy,-1));

		xd = (DBR & xi) + (DR & ux);
		yd = (DBR & yi) + (DR & uy);

		x = x.addBlock(x, xd, 1, 1, qval, 1);
		x = x.addBlock(x, xi, qval+1, 1, ch - qval, 1);
		y = y.addBlock(y, yd, 1, 1, qval, 1);
		y = y.addBlock(y, yi, qval+1, 1, ch - qval, 1);
		//
		//
		// mapping the x and y back
		x = x.conversionVectorback(x, index);
		y = y.conversionVectorback(y, index);
#pragma endregion
		
		myRegion.root = myRegion.updateModule(myRegion.root, x, y, Num_Mcells) ;
		*myRegion.netArray = myRegion.updateNetArray(myRegion.root, myRegion.netArray);
}

#pragma endregion


#pragma region  assistant functions

/***************************   update the positions of the modules  *******************/
module *region::updateModule(module *temp, Matrix &matx, Matrix &maty, int n) 
{
	// need mapping   !!!!!!!!!!!!!!!!!!!
	module *w = temp;
	int i = 1;
	while(w != NULL)
	{
		if(w->MorF == 'M')
		{
			w->moduleX = static_cast<float>(matx(i,1));
			w->moduleY = static_cast<float>(maty(i,1));
			i++;
		}
		w = w->next;
	}
	return temp;
}

/*********************    count the total number of modules in a net   ****************************/
int MyForm::contPin(module *net)
{
	int cont = 0;
	while(net != NULL)
	{
		net = net->next;
		cont++;
	}
	return cont;
}

/*********************    read the net Array to get the information   ****************************/
void region::readNetArray(module *net[], int nn)
{
	module *w;
	for(int i=1; i <= nn; i++)
	{
		w = net[i];
		while( w != NULL)
		{
			w = w->next;// reach the end
		}
	}
}

/*****************    read the net list and construct a linklist contains all infomations about the netlist   ******************/
Void MyForm::button2_Click(System::Object^  sender, System::EventArgs^  e) 
{
	//cleanRoot();
	txtRead(FILENAME);// analize the net list
}

/**********************   read the net list file   **********************************/
void MyForm::txtRead(String^ filename)  
{   
	//define a myRegion
	//
	//

#pragma region   read the netlist

	try
	{
	// Create an instance of StreamReader to read from a file.
      StreamReader^ sr = gcnew StreamReader( filename );
      try
      {
         String^ line;
         // Read and display lines from the file until the end of 
         // the file is reached.
         while ( line = sr->ReadLine() )
         {
			 if(line == "\n\r")
			 {
				 MessageBox::Show(this,"I suck");
				 break;
			 }
			 //MessageBox::Show(this,line);
			 string a;
			 MarshalString(line, a);
			 if( checkLine(a) == 'M')//chech every line and do different operations 
			 {
				 // check if the module is defined before, if yes, add a pin to that module, else create another new module
				myRegion.checkIfUse(myRegion.root, moduleNum, netNum, pinX, pinY, weight);
				myRegion.root = myRegion.createModule(myRegion.root, moduleNum, netNum, weight, moduleW, moduleH, moduleX, moduleY, pinX, pinY, MorF);
				//drawNew(root);
			 }
			 Console::WriteLine( line );
         }
      }
      finally
      {
         if ( sr )
            delete (IDisposable^)sr;
      }
   }
   catch ( Exception^ e ) 
   {
      // Let the user know what went wrong.
	  MessageBox::Show(this,"I suck");
      Console::WriteLine( "The file could not be read:" );
      Console::WriteLine( e->Message );
   }
#pragma endregion
 
    myRegion.root = myRegion.mapMovable(myRegion.root);// mapping the movable modules
    *myRegion.netArray = myRegion.createNetArray(myRegion.root, myRegion.netArray);// create a pointer array based on nets
	//drawNetArray(netArray);
    drawModule(myRegion.root);
	//MessageBox::Show(this,"Read completed!");

#pragma region   create the gordian button

	// 
	// button1, GORDIAN
	// 
	this->button1 = (gcnew System::Windows::Forms::Button());
	this->button1->Location = System::Drawing::Point(rootW + buttonM + 2, formH - buttonH);
	this->button1->Name = L"Gordian";
	this->button1->Size = System::Drawing::Size((formW - rootW -2)/3, buttonH);
	this->button1->TabIndex = 0;
	this->button1->Text = L"Gordian";
	this->button1->UseVisualStyleBackColor = true;
	this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
	this->Controls->Add(this->button1);
	// 
	// button3, wire
	// 
	this->button3 = (gcnew System::Windows::Forms::Button());
	this->button3->Location = System::Drawing::Point(rootW + buttonM * 2 + 2, formH - buttonH);
	this->button3->Name = L"Wire";
	this->button3->Size = System::Drawing::Size((formW - rootW -2)/3, buttonH);
	this->button3->TabIndex = 0;
	this->button3->Text = L"Wire";
	this->button3->UseVisualStyleBackColor = true;
	this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
	this->Controls->Add(this->button3);
#pragma endregion

}

/*********************    map the movable modules   ************************/
module *region::mapMovable(module *temp)
{
	int contM = 1;// start from 1
	int contF = 1;// start from 1
	module *w = temp;
	while(w != NULL)
	{
		if(w->MorF == 'M')
		{
			w->mapNum = contM;// mapping
			contM++;
		}
		else
		{
			w->mapNum = contF;// mapping
			contF++;
		}
		w = w->next;
	}
	return temp;
}


/*********************    draw netArray   ****************************/
void MyForm::drawWire(module *net[])
{
	module *w;
	for(int i=1; i <= Num_Nets; i++)
	{
		w = net[i];
		float middleX = 0;
		float middleY = 0;
		float num = 0;
		while( w != NULL)//calculate the center position
		{
			w->pinX[1] = w->moduleX  + w->pinX[0];
			w->pinY[1] = w->moduleY  + w->pinY[0];
			middleX += w->pinX[1];
			middleY += w->pinY[1];
			num++;
			w = w->next;
		}
		w = net[i];// reset the w position
		middleX /= num;// calculate the average
		middleY /= num;
		while( w != NULL)
		{
			DrawLineLine(w->pinX[1], w->pinY[1], middleX, middleY);
			totalLengh += sqrt(
				(middleX - w->pinX[1])*(middleX - w->pinX[1])
				+(middleY - w->pinY[1])*(middleY - w->pinY[1]));
			w = w->next;// reach the end
		}
	}
}

/*********************    draw netArray   ****************************/
void MyForm::drawNetArray(module *net[])
{
	cleanRoot();
	module *w;
	for(int i=1; i <= Num_Nets; i++)
	{
		w = net[i];
		while( w != NULL)
		{
			DrawRectangleRectangle(w->moduleX - w->moduleW/2, w->moduleY - w->moduleH/2, w->moduleW, w->moduleH);
			w = w->next;// reach the end
		}
	}
}
/******************** create a net link list ********************************/
module *region::updateNetArray(module *temp, module *net[])
{
	while(temp != NULL)// it's a module in the module link list
	{
		for(int i = 0; i < (temp->netSum); i++)// add to all the nets it is connected to
		{
			net[temp->netNum[i]] = updatePosition(temp, net[temp->netNum[i]], i);
		}
		temp = temp->next;
	}
	return *net;
}

/******************** insert a module to then end of a link list *******************/
module *region::updatePosition(module *temp, module *net, int n)
{
	module *w = net;
	while(w->moduleNum != temp->moduleNum)// reach the module
		w = w->next;

	w->moduleX = temp->moduleX;// the x position of the module
	w->moduleY = temp->moduleY;// the y position of the module

	return net;
}

/******************** create a net link list ********************************/
module *region::createNetArray(module *temp, module *net[])
{
	while(temp != NULL)// it's a module
	{
		for(int i = 0; i < (temp->netSum); i++)// add to all the nets it is connected to
		{
			net[temp->netNum[i]] = insertNet(temp, net[temp->netNum[i]], i);
		}
		temp = temp->next;
	}
	return *net;
}

/******************** insert a module to then end of a link list *******************/
module *region::insertNet(module *temp, module *net, int n)
{
	module *newModule=new module;//create a new module
	newModule->moduleNum = temp->moduleNum;
	newModule->mapNum = temp->mapNum;
	newModule->weight[0] = temp->weight[n];
	newModule->moduleW = temp->moduleW;
	newModule->area = temp->area;
	newModule->moduleH = temp->moduleH;// height
	newModule->moduleX = temp->moduleX;// the x position of the module
	newModule->moduleY = temp->moduleY;// the y position of the module
	newModule->pinNum = temp->pinNum;// the total number of the pins, initial value should be 1
	newModule->netSum = temp->netSum;// the total number of the nets it is connected to
	newModule->pinX[0] = temp->pinX[n];// the x position of the pin
	newModule->pinY[0] = temp->pinY[n];// the y position of the pin
	newModule->MorF = temp->MorF;// movable or fixed
	newModule->next = NULL;// link to another module
		
	if( net == NULL)
	{
		net = newModule;
		return net;
	}
	else
	{
		module *w = net;
		while(w->next != NULL)// reach the end
			w = w->next;
		w->next = newModule;
		return net;
	}
}

/******************** draw the modules by searching the list    *********************/
void MyForm::drawModule(module *temp)
{
	while(temp != NULL)
	{
		DrawRectangleRectangle(temp->moduleX - temp->moduleW/2, temp->moduleY - temp->moduleH/2, temp->moduleW, temp->moduleH);
		temp = temp->next;// reach the end
	}
}

/********************* it's a test case and should be deleted after the debuging    *************/
void MyForm::drawNew(module *temp)
{
	cleanRoot();
	while(temp->next != NULL)
		temp = temp->next;// reach the end
	DrawRectangleRectangle(temp->moduleX - temp->moduleW/2, temp->moduleY - temp->moduleH/2, temp->moduleW, temp->moduleH);
}

/**********************   Check if the module has been defined before   *****************************/
int region::checkIfUse(module * temp, int mN, int nN, int pX, int pY, int w)
{
	if( temp != NULL)// no modules
	{
		if( temp->moduleNum == mN )// find it has been defined
		{
			addOnePin(temp, nN, pX, pY, w);
			addTF = true;// flag the add
			return 1;
		}
		checkIfUse(temp->next, mN, nN, pX, pY, w);// find the next net
	}
	return 0;
}

/********************   create a module to record the data in a tree  **********************/
module *region::createModule(module *temp, int mN, int nN, int w, int mW, int mH, float mX, float mY, int pX, int pY, char MF)
{
	if(addTF)
	{
		addTF = false;// clean the flag
		return temp;// the module has been defined, do not create a new one
	}
	else
	{
		module *newModule=new module;//create a new gate
		newModule->moduleNum = mN;
		newModule->moduleW = static_cast<float>(mW);
		newModule->moduleH = static_cast<float>(mH);// height
		newModule->area = newModule->moduleW * newModule->moduleH;
		newModule->moduleX = (mX);// the x position of the module
		newModule->moduleY = (mY);// the y position of the module
		newModule->pinNum = 1;// the total number of the pins, initial value should be 1
		newModule->netSum = 1;// the total number of the nets it is connected to 
		newModule->netNum[ newModule->pinNum - 1 ]= nN;
		newModule->weight[ newModule->pinNum -1 ] = (w);
		newModule->pinX[ newModule->pinNum -1 ] = static_cast<float>(pX);// the x position of the pin
		newModule->pinY[ newModule->pinNum -1 ] = static_cast<float>(pY);// the y position of the pin
		newModule->MorF = MF;// movable or fixed
		newModule->next = NULL;// link to another module
		if(temp == NULL)
		{
			return newModule;// create a root module
		}
		else
		{
			module *root = temp;//record the root 
			while(temp->next != NULL)// reach the end of the list
				temp = temp->next;
			temp->next = newModule;
			return root;
		}
	}
}

/*******************************    add a pin to a exited module   ***********************/
void region::addOnePin(module *temp, int nN, int pX, int pY, int w)
{
	temp->pinNum++;
	temp->netSum++;
	temp->netNum[ temp->pinNum -1 ]= nN;
	temp->weight[ temp->pinNum -1 ]= w;
	temp->pinX[temp->pinNum -1 ] = static_cast<float>(pX);
	temp->pinY[temp->pinNum -1 ] = static_cast<float>(pY);
}

/**********************   Check the lines int the netlist to find layout information    ************************/
char MyForm::checkLine (string s)
{
	#pragma region
	if(s[4] == 'C')// it defines the number of the total cells
	{ 
		Num_Cells = getNum(s);
	}
	else if(s[4] == 'M')// it defines the number of the total movable cells
	{ 
		Num_Mcells = getNum(s);
	}
	else if(s[4] == 'F')// it defines the number of the total fixed cells
	{ 
		Num_Fcells = getNum(s);
	}
	else if(s[4] == 'N')// it defines the number of the total nets
	{ 
		Num_Nets = getNum(s);
	}
	else if(s[0] == 'W')// it defines the width of the root region
	{ 
		W = getNum(s);
	}
	else if(s[0] == 'H')// it defines the height of the root region
	{ 
		H = getNum(s);
		ratio = rootH / H - 1;
		endx = static_cast<float>(W * ratio);// calculate the place of the root region
		endy = static_cast<float>(H * ratio);
		startx = (rootW - endx)/2; 
		starty = (rootH - endy)/2;
		myRegion.divide = new partition;//create the root region partision
		myRegion.divide->sx = startx;
		myRegion.divide->sy = starty;
		myRegion.divide->ex = startx + endx;
		myRegion.divide->ey = starty + endy;
		myRegion.divide->used = false;
		myRegion.divide->left = NULL;
		myRegion.divide->right = NULL;
		//endx += startx;
		//endy += starty;
		cleanRoot();
	}
	else if(s[0] == 'N')// it defines the width of the root region
	{ 
		weight = getWeight(s);
		netNum = getNetNum(s);
	}
	#pragma endregion
	else // it is a module
	{
		moduleNum = getModuleInfo(s, '(', 1, ')', 1);// the module name
		moduleW = ratio * getModuleInfo(s, '(', 2, ',', 1);// width 
		moduleH = ratio * getModuleInfo(s, ',', 1, ')', 2);// height
		pinX = ratio * getModuleInfo(s, '(', 3, ')', 2);// the x position of the pin
		pinY = ratio * getModuleInfo(s, ',', 2, ')', 3);// the y position of the pin
		if( getModuleType(s) == 'M')
		{
			MorF = 'M';// movable 
			moduleX = startx;//rand() % (W * ratio ) + 1;;// the x position of the module
			moduleY = startx;//rand() % (H * ratio ) + 1;;// the y position of the module
		}
		else
		{
			MorF = 'F';
			moduleX = startx + ratio * getModuleInfo(s, '(', 4, ',', 3);// the x position of the module
			moduleY = starty + ratio * getModuleInfo(s, ',', 3, ')', 4);// the y position of the module
		}
		return 'M';
	}
	return 0;
}

#pragma region handling the strings to get the information needed
	
/********************   get the num information about the netlist  *************************/
int MyForm::getModuleInfo(string s, char a, int ac, char b, int bc)
{
	int start;//
	int end;//
	int contStart = 0;
	int contEnd = 0;
	string str;
	for(start = 0; contStart != ac; start++ )// reach the start of the number
	{
		if( s[start] == a )
			contStart++;
	}
	for(end = 0; contEnd != bc; end++ )// reach the end of the number
	{
		if( s[end] == b )
			contEnd++;
	}
	str = s.substr(start, end - 2);
	return stringToNum<int>(str);
}

/********************   get the num information about the netlist  *************************/
int MyForm::getNetNum(string s)
{
	int length;
	length = s.length();// get the length of the line
	string str;
	str = s.substr(2,length -2);
	return stringToNum<int>(str);
}

/********************   get the num information about the netlist  *************************/
int MyForm::getNum(string s)
{
	int length;
	length = s.length();// get the length of the line
	int i;
	string str;
	for(i=0; s[i] != '='; i++);// reach the '='
	str = s.substr(i + 1,length -1);
	return stringToNum<int>(str);
}

/********************   get the num information about the netlist  *************************/
int MyForm::getWeight(string s)
{
	int length;
	length = s.length();// get the length of the line
	int i;
	int cont=0;
	string str;
	for(i=0; cont != 2 && i<(length - 1); i++)// reach the second '('
	{
		if(s[i] == '(')
			cont++;
	}
	if(cont == 1)
		return 1;// default value
	else
	{
		str = s.substr(i ,length -2);
		return stringToNum<int>(str);
	}
}

/********************   get the num information about the netlist  *************************/
char MyForm::getModuleType(string s)
{
	int length;
	length = s.length();// get the length of the line
	char a = s[length - 1];
	return s[length - 1];
}

/*****************   change the String to string   *******************/
Void MyForm::MarshalString ( String ^ s, string& os ) 
{
   using namespace Runtime::InteropServices;
   const char* chars = 
      (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
   os = chars;
   Marshal::FreeHGlobal(IntPtr((void*)chars));
}

/********************    transfer the string to integer    ********************/
template <class Type>
Type MyForm::stringToNum(const string& str)
{
istringstream iss(str);
Type num;
iss >> num;
return num;
}

#pragma endregion

/**********************   Draw the module to the region   ***************************/
Void MyForm::cleanRoot(void)//System::Drawing::Rectangle^ rect)
{
	//Pen^ pen = gcnew Pen(Color::Black);
	SolidBrush^ Brush = gcnew SolidBrush( Color::White);
	Graphics^ formGraphics;
	formGraphics = this->CreateGraphics();
	formGraphics->FillRectangle(Brush, 0, 0, rootH, rootH);
	delete Brush;
	delete formGraphics;
	DrawRectangleRectangle(startx , starty, endx, endy);
}

/**********************   Draw the module to the region   ***************************/
Void MyForm::DrawRectangleRectangle(float a, float b, float c, float d)//System::Drawing::Rectangle^ rect)
{
	Pen^ pen = gcnew Pen(Color::Black);
	Graphics^ formGraphics;
	formGraphics = this->CreateGraphics();
	formGraphics->DrawRectangle(pen, a, b, c, d);
	delete pen;
	delete formGraphics;
}

/**********************   Draw the line to the region   ***************************/
Void MyForm::DrawLineLine(float a, float b, float c, float d)
{
	
	System::Drawing::Pen^ pen =
	gcnew System::Drawing::Pen(System::Drawing::Color::Green);
	System::Drawing::Graphics^ formGraphics;
	formGraphics = this->CreateGraphics();
	formGraphics->DrawLine(pen, a, b, c, d);
	delete pen;
	delete formGraphics;
}

#pragma endregion
