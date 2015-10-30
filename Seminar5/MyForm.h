#pragma once

/*
=================== Проект 5. 3D Part 2 =====================

Задание: Семинар 5
Автор: Куликов Андрей, БПИ121
Дата: 25.10.2015

Реализованный функционал:
1) Формирование матрицы вершин и граней трехмерных полигональных объектов -
платоновых тел тетраэдра, октаэдра, икосаэдра
2)  Формирование матрицы вершин и граней трехмерных объектов - сфера, тор, капля, 
пружина (вдоль оси y)
3) Вывод объектов на экран, обеспечение возможности их вращения, сдвига,
масштабирования, параллельного и перспективного проецирования
4) Вращение относительно прямой, параллельной оси координат x
5) Ввращение относительно произвольно заданной прямой L, проходящей через
точку A(a,b,c) и имеющую направляющий вектор (l,m,n)
6) Три основные ортографические проекции (вид спереди, сверху, сбоку)
7) Управление мышью

Среда разработки: Visual Studio 2013
*/

namespace Seminar5 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Text;
	using namespace System::Globalization;

	using namespace System::Collections::Generic;

#pragma region Addition Classes
	public ref class Matrix
	{
	private:
		int m, n;
		array<double, 2>^ arr;
	public:
		Matrix(int n, int m);
		Matrix(array<double, 2>^);
		Matrix(array<array<double>^>^);
		Matrix(Matrix^ other);
		Matrix(int n, int m, ...array<double>^ coords);
	public:
		static Matrix^ operator +(const Matrix^, const Matrix^);
		static Matrix^ operator -(const Matrix^, const Matrix^);
		static Matrix^ operator *(Matrix^, Matrix^);
		static Matrix^ operator *(const int, const Matrix^);
		property double default[int, int] {
			double get(int i, int j)
			{
				return arr[i, j];
			}
			void set(int i, int j, double value)
			{
				arr[i, j] = value;
			}
		}
		property int Rows
		{
			int get()
			{
				return m;
			}
		}
		property int Columns
		{
			int get()
			{
				return n;
			}
		}
		Matrix^ normalize();
		Matrix^ set(Matrix^ other);
		virtual String^  ToString() override;
	};

	//отвечает за выполнение операций трансформации и проекций
	public ref class GeomHelper {
	private:
		static double  degToRad(double deg);
	public:
		enum class Projection
		{
			Parallel, Central,
			Front, Top, Side
		};
		static void rotateX(Matrix^ transforms, double angleDegrees);
		static void rotateX(Matrix^ transforms, double m, double n, bool inverseMatrix);
		static void rotateY(Matrix^ transforms, double angleDegrees);
		static void rotateY(Matrix^ transforms, double l, double m, double n, bool inverseMatrix);
		static void rotateZ(Matrix^ transforms, double angleDegrees);
		static void reflectX(Matrix^ transforms);
		static void reflectY(Matrix^ transforms);
		static void reflectZ(Matrix^ transforms);
		static void translate(Matrix^ transforms, int dx, int dy, int dz);
		static void scale(Matrix^ transforms, double p, double q, double r, double s);
		static void project(Matrix^ transforms, Matrix^ projection, Projection projMode, int dist);
	};

	public ref  class Figure abstract
	{
	protected:
		Matrix ^vertices, ^facets;
		int size = 50;
	public:
		property Matrix^ Vertices {Matrix^ get() { return vertices; } };
		property Matrix^ Facets {Matrix^ get() { return facets; } }
	};

	public ref class Sphere : Figure{
	private:
		static Sphere^ instance;
		int  radius = 10;
		Sphere();
	public:
		static property Sphere^ Instance {Sphere^ get(); };


	};
	public ref class Torus : Figure {
	private:
		static Torus^ instance;
		int rBig = 12, rSmall = 5;
		Torus();
	public:
		static property Torus^ Instance {Torus^ get(); };

	};
	public ref class Tetrahedron : Figure {
	private:
		static Tetrahedron^ instance;
		Tetrahedron();
	public:
		static property Tetrahedron^ Instance {Tetrahedron^ get(); };

	};
	public ref class Octahedron : Figure {
	private:
		static Octahedron^ instance;
		Octahedron();
	public:
		static property Octahedron^ Instance {Octahedron^ get(); };

	};
	public ref class Icosahedron : Figure {
	private:
		static Icosahedron^ instance;
		Icosahedron();
	public:
		static property Icosahedron^ Instance {Icosahedron^ get(); };
	};
	public ref class Drop : Figure {
	private:
		static Drop^ instance;
		int  radius = 10;
		Drop();
	public:
		static property Drop^ Instance {Drop^ get(); };
	};
	public ref class Spring : Figure {
	private:
		static Spring^ instance;
		int rBig = 12, rSmall = 5;
		Spring();
	public:
		static property Spring^ Instance {Spring^ get(); };

	};

	//Отвечает за моделирование платоновых тел и тел вращения
	public ref class GraphicsStorage
	{
	private:
		static Figure^ figure;
		static Pen^ linePen;
		static int size;
	public:
		static GraphicsStorage();
		static Matrix^ vertices;
		static Matrix^ projection;
		static void redraw(Graphics^, int centerX, int centerY, GeomHelper::Projection projection, int z0);
		static void setFigure(Figure^ figure);
		static void resetVertices();
	};

	public ref class UIState
	{
	public:
		static GeomHelper::Projection projection = GeomHelper::Projection::Parallel;
		static double dist = 1000;
		static array<int>^ offsets = gcnew array<int>(3);
		static array<int>^ rotations = gcnew array<int>(3);
		static array<double>^ customPoint = gcnew array<double>(3);
		static array<double>^ customVector = gcnew array<double>(3);
		static int delay = 80;
		static array<double>^ scales = gcnew array<double>(4) { 1, 1, 1, 1 }; //+all
		static bool leftBtnPressed = false;
		static bool rightBtnPressed = false;

	};

#pragma endregion

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	private:
		enum class TranslationPlane
		{
			XY, XZ, YZ
		};
		enum class RotationPlane
		{
			XY, XZ, YZ
		};
		enum class ScaleAxis
		{
			X, Y, Z, XYZ
		};

		array<int>^ tempValues = gcnew array<int>(3);

	public:
		MyForm(void)
		{
			InitializeComponent();

			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  mainTabControl;
	protected:

	protected:


	private: System::Windows::Forms::TabPage^  tabMouseSettings;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutMouseSettings;
	private: System::Windows::Forms::GroupBox^  gbWheelMouse;
	private: System::Windows::Forms::GroupBox^  gbRightMouse;
	private: System::Windows::Forms::GroupBox^  gbLeftMouse;
	private: System::Windows::Forms::FlowLayoutPanel^  flpLeftMouse;
	private: System::Windows::Forms::RadioButton^  rbLeftMouseXY;
	private: System::Windows::Forms::RadioButton^  rbLeftMouseXZ;
	private: System::Windows::Forms::RadioButton^  rbLeftMouseYZ;
	private: System::Windows::Forms::TabPage^  tabFigure;

	private: System::Windows::Forms::FlowLayoutPanel^  flpWheelMouse;
	private: System::Windows::Forms::RadioButton^  rbWheelMouseX;
	private: System::Windows::Forms::RadioButton^  rbWheelMouseY;
	private: System::Windows::Forms::RadioButton^  rbWheelMouseZ;



	private: System::Windows::Forms::FlowLayoutPanel^  flpRightMouse;
	private: System::Windows::Forms::RadioButton^  rbRightMouseXY;
	private: System::Windows::Forms::RadioButton^  rbRightMouseXZ;
	private: System::Windows::Forms::RadioButton^  rbRightMouseYZ;
	private: System::Windows::Forms::TableLayoutPanel^  rootTableLayoutPanel;
	private: System::Windows::Forms::PictureBox^  pictureBox1;


	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		Bitmap^ bitmap;
		Graphics^ graphics;
		TranslationPlane translationPlane = TranslationPlane::XY;
		RotationPlane rotationPlane = RotationPlane::XY;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutFigureSettings;
	private: System::Windows::Forms::GroupBox^  gbSelectFigure;
	private: System::Windows::Forms::ComboBox^  comboBoxFigure;
	private: System::Windows::Forms::TabPage^  tabRotation;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutRotationSettings;
	private: System::Windows::Forms::GroupBox^  gbCustomLineRotation;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutCustomLineRotation;
	private: System::Windows::Forms::Label^  lblForCustomVectorRotation;
	private: System::Windows::Forms::TextBox^  tbCustomPointRotation;

	private: System::Windows::Forms::TextBox^  tbCustomVectorRotation;
	private: System::Windows::Forms::Label^  lblForCustomPointRotation;

	private: System::Windows::Forms::Button^  btnStartCustomLineRotation;
	private: System::ComponentModel::BackgroundWorker^  bgWorkerRotation;
	private: System::Windows::Forms::GroupBox^  gbProjectionSettings;
	private: System::Windows::Forms::FlowLayoutPanel^  flpProjectionSettings;
	private: System::Windows::Forms::RadioButton^  rbParallel;
	private: System::Windows::Forms::RadioButton^  rbCentral;
	private: System::Windows::Forms::FlowLayoutPanel^  flpZ0Settings;
	private: System::Windows::Forms::Label^  lblZ0Settings;
	private: System::Windows::Forms::TextBox^  tbZ0Settings;
	private: System::Windows::Forms::RadioButton^  rbWheelMouseXYZ;
	private: System::Windows::Forms::GroupBox^  gbFigureView;
	private: System::Windows::Forms::FlowLayoutPanel^  flpFigureView;

	private: System::Windows::Forms::RadioButton^  rbFrontFigureView;
	private: System::Windows::Forms::RadioButton^  rbTopFigureView;
	private: System::Windows::Forms::RadioButton^  rbSideFigureView;



	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::Button^  btnResetFigure;
	private: System::Windows::Forms::TextBox^  tbAnimationDelay;
	private: System::Windows::Forms::Label^  lblForAnimationDelay;
	private: System::Windows::Forms::Label^  lblForStartCustomLineRotation;
private: System::Windows::Forms::TabPage^  tabAboutMe;
private: System::Windows::Forms::Label^  lblAboutMe;




			 ScaleAxis scaleAxis = ScaleAxis::XYZ;

			 void redraw(GeomHelper::Projection projection, int z0)
			 {
				 GraphicsStorage::redraw(graphics, pictureBox1->Width / 2, pictureBox1->Height / 2, projection, z0);
				 pictureBox1->Refresh();

			 }

			 void onSizeChaged()
			 {
				 bitmap = gcnew Bitmap(pictureBox1->Width, pictureBox1->Height);
				 pictureBox1->Image = bitmap;
				 graphics = Graphics::FromImage(bitmap);
				 graphics->Clear(Color::Yellow);
				 redraw(UIState::projection, UIState::dist);
			 }



#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// Required method for Designer support - do not modify
			 /// the contents of this method with the code editor.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
				 this->mainTabControl = (gcnew System::Windows::Forms::TabControl());
				 this->tabMouseSettings = (gcnew System::Windows::Forms::TabPage());
				 this->tableLayoutMouseSettings = (gcnew System::Windows::Forms::TableLayoutPanel());
				 this->gbProjectionSettings = (gcnew System::Windows::Forms::GroupBox());
				 this->flpProjectionSettings = (gcnew System::Windows::Forms::FlowLayoutPanel());
				 this->rbParallel = (gcnew System::Windows::Forms::RadioButton());
				 this->rbCentral = (gcnew System::Windows::Forms::RadioButton());
				 this->flpZ0Settings = (gcnew System::Windows::Forms::FlowLayoutPanel());
				 this->lblZ0Settings = (gcnew System::Windows::Forms::Label());
				 this->tbZ0Settings = (gcnew System::Windows::Forms::TextBox());
				 this->gbWheelMouse = (gcnew System::Windows::Forms::GroupBox());
				 this->flpWheelMouse = (gcnew System::Windows::Forms::FlowLayoutPanel());
				 this->rbWheelMouseX = (gcnew System::Windows::Forms::RadioButton());
				 this->rbWheelMouseY = (gcnew System::Windows::Forms::RadioButton());
				 this->rbWheelMouseZ = (gcnew System::Windows::Forms::RadioButton());
				 this->rbWheelMouseXYZ = (gcnew System::Windows::Forms::RadioButton());
				 this->gbRightMouse = (gcnew System::Windows::Forms::GroupBox());
				 this->flpRightMouse = (gcnew System::Windows::Forms::FlowLayoutPanel());
				 this->rbRightMouseXY = (gcnew System::Windows::Forms::RadioButton());
				 this->rbRightMouseXZ = (gcnew System::Windows::Forms::RadioButton());
				 this->rbRightMouseYZ = (gcnew System::Windows::Forms::RadioButton());
				 this->gbLeftMouse = (gcnew System::Windows::Forms::GroupBox());
				 this->flpLeftMouse = (gcnew System::Windows::Forms::FlowLayoutPanel());
				 this->rbLeftMouseXY = (gcnew System::Windows::Forms::RadioButton());
				 this->rbLeftMouseXZ = (gcnew System::Windows::Forms::RadioButton());
				 this->rbLeftMouseYZ = (gcnew System::Windows::Forms::RadioButton());
				 this->tabFigure = (gcnew System::Windows::Forms::TabPage());
				 this->tableLayoutFigureSettings = (gcnew System::Windows::Forms::TableLayoutPanel());
				 this->gbFigureView = (gcnew System::Windows::Forms::GroupBox());
				 this->flpFigureView = (gcnew System::Windows::Forms::FlowLayoutPanel());
				 this->rbFrontFigureView = (gcnew System::Windows::Forms::RadioButton());
				 this->rbTopFigureView = (gcnew System::Windows::Forms::RadioButton());
				 this->rbSideFigureView = (gcnew System::Windows::Forms::RadioButton());
				 this->gbSelectFigure = (gcnew System::Windows::Forms::GroupBox());
				 this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
				 this->comboBoxFigure = (gcnew System::Windows::Forms::ComboBox());
				 this->btnResetFigure = (gcnew System::Windows::Forms::Button());
				 this->tabRotation = (gcnew System::Windows::Forms::TabPage());
				 this->tableLayoutRotationSettings = (gcnew System::Windows::Forms::TableLayoutPanel());
				 this->gbCustomLineRotation = (gcnew System::Windows::Forms::GroupBox());
				 this->tableLayoutCustomLineRotation = (gcnew System::Windows::Forms::TableLayoutPanel());
				 this->tbAnimationDelay = (gcnew System::Windows::Forms::TextBox());
				 this->lblForAnimationDelay = (gcnew System::Windows::Forms::Label());
				 this->lblForCustomVectorRotation = (gcnew System::Windows::Forms::Label());
				 this->tbCustomPointRotation = (gcnew System::Windows::Forms::TextBox());
				 this->tbCustomVectorRotation = (gcnew System::Windows::Forms::TextBox());
				 this->lblForCustomPointRotation = (gcnew System::Windows::Forms::Label());
				 this->btnStartCustomLineRotation = (gcnew System::Windows::Forms::Button());
				 this->lblForStartCustomLineRotation = (gcnew System::Windows::Forms::Label());
				 this->rootTableLayoutPanel = (gcnew System::Windows::Forms::TableLayoutPanel());
				 this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
				 this->bgWorkerRotation = (gcnew System::ComponentModel::BackgroundWorker());
				 this->tabAboutMe = (gcnew System::Windows::Forms::TabPage());
				 this->lblAboutMe = (gcnew System::Windows::Forms::Label());
				 this->mainTabControl->SuspendLayout();
				 this->tabMouseSettings->SuspendLayout();
				 this->tableLayoutMouseSettings->SuspendLayout();
				 this->gbProjectionSettings->SuspendLayout();
				 this->flpProjectionSettings->SuspendLayout();
				 this->flpZ0Settings->SuspendLayout();
				 this->gbWheelMouse->SuspendLayout();
				 this->flpWheelMouse->SuspendLayout();
				 this->gbRightMouse->SuspendLayout();
				 this->flpRightMouse->SuspendLayout();
				 this->gbLeftMouse->SuspendLayout();
				 this->flpLeftMouse->SuspendLayout();
				 this->tabFigure->SuspendLayout();
				 this->tableLayoutFigureSettings->SuspendLayout();
				 this->gbFigureView->SuspendLayout();
				 this->flpFigureView->SuspendLayout();
				 this->gbSelectFigure->SuspendLayout();
				 this->flowLayoutPanel1->SuspendLayout();
				 this->tabRotation->SuspendLayout();
				 this->tableLayoutRotationSettings->SuspendLayout();
				 this->gbCustomLineRotation->SuspendLayout();
				 this->tableLayoutCustomLineRotation->SuspendLayout();
				 this->rootTableLayoutPanel->SuspendLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
				 this->tabAboutMe->SuspendLayout();
				 this->SuspendLayout();
				 // 
				 // mainTabControl
				 // 
				 this->mainTabControl->Controls->Add(this->tabMouseSettings);
				 this->mainTabControl->Controls->Add(this->tabFigure);
				 this->mainTabControl->Controls->Add(this->tabRotation);
				 this->mainTabControl->Controls->Add(this->tabAboutMe);
				 this->mainTabControl->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->mainTabControl->Location = System::Drawing::Point(3, 3);
				 this->mainTabControl->MinimumSize = System::Drawing::Size(0, 150);
				 this->mainTabControl->Name = L"mainTabControl";
				 this->mainTabControl->SelectedIndex = 0;
				 this->mainTabControl->Size = System::Drawing::Size(645, 150);
				 this->mainTabControl->TabIndex = 1;
				 // 
				 // tabMouseSettings
				 // 
				 this->tabMouseSettings->Controls->Add(this->tableLayoutMouseSettings);
				 this->tabMouseSettings->Location = System::Drawing::Point(4, 22);
				 this->tabMouseSettings->Name = L"tabMouseSettings";
				 this->tabMouseSettings->Padding = System::Windows::Forms::Padding(3);
				 this->tabMouseSettings->Size = System::Drawing::Size(637, 124);
				 this->tabMouseSettings->TabIndex = 0;
				 this->tabMouseSettings->Text = L"Настройки мыши";
				 this->tabMouseSettings->UseVisualStyleBackColor = true;
				 // 
				 // tableLayoutMouseSettings
				 // 
				 this->tableLayoutMouseSettings->ColumnCount = 4;
				 this->tableLayoutMouseSettings->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
					 25)));
				 this->tableLayoutMouseSettings->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
					 25)));
				 this->tableLayoutMouseSettings->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
					 25)));
				 this->tableLayoutMouseSettings->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
					 25)));
				 this->tableLayoutMouseSettings->Controls->Add(this->gbProjectionSettings, 3, 0);
				 this->tableLayoutMouseSettings->Controls->Add(this->gbWheelMouse, 2, 0);
				 this->tableLayoutMouseSettings->Controls->Add(this->gbRightMouse, 1, 0);
				 this->tableLayoutMouseSettings->Controls->Add(this->gbLeftMouse, 0, 0);
				 this->tableLayoutMouseSettings->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->tableLayoutMouseSettings->Location = System::Drawing::Point(3, 3);
				 this->tableLayoutMouseSettings->Name = L"tableLayoutMouseSettings";
				 this->tableLayoutMouseSettings->RowCount = 1;
				 this->tableLayoutMouseSettings->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent,
					 100)));
				 this->tableLayoutMouseSettings->Size = System::Drawing::Size(631, 118);
				 this->tableLayoutMouseSettings->TabIndex = 0;
				 // 
				 // gbProjectionSettings
				 // 
				 this->gbProjectionSettings->Controls->Add(this->flpProjectionSettings);
				 this->gbProjectionSettings->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->gbProjectionSettings->Location = System::Drawing::Point(474, 3);
				 this->gbProjectionSettings->Name = L"gbProjectionSettings";
				 this->gbProjectionSettings->Size = System::Drawing::Size(154, 112);
				 this->gbProjectionSettings->TabIndex = 3;
				 this->gbProjectionSettings->TabStop = false;
				 this->gbProjectionSettings->Text = L"Проекция";
				 // 
				 // flpProjectionSettings
				 // 
				 this->flpProjectionSettings->AutoSize = true;
				 this->flpProjectionSettings->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
				 this->flpProjectionSettings->Controls->Add(this->rbParallel);
				 this->flpProjectionSettings->Controls->Add(this->rbCentral);
				 this->flpProjectionSettings->Controls->Add(this->flpZ0Settings);
				 this->flpProjectionSettings->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->flpProjectionSettings->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
				 this->flpProjectionSettings->Location = System::Drawing::Point(3, 16);
				 this->flpProjectionSettings->Name = L"flpProjectionSettings";
				 this->flpProjectionSettings->Size = System::Drawing::Size(148, 93);
				 this->flpProjectionSettings->TabIndex = 3;
				 // 
				 // rbParallel
				 // 
				 this->rbParallel->AutoSize = true;
				 this->rbParallel->Checked = true;
				 this->rbParallel->Location = System::Drawing::Point(3, 3);
				 this->rbParallel->Name = L"rbParallel";
				 this->rbParallel->Size = System::Drawing::Size(99, 17);
				 this->rbParallel->TabIndex = 0;
				 this->rbParallel->TabStop = true;
				 this->rbParallel->Text = L"Параллельная";
				 this->rbParallel->UseVisualStyleBackColor = true;
				 this->rbParallel->CheckedChanged += gcnew System::EventHandler(this, &MyForm::rbParallel_CheckedChanged);
				 // 
				 // rbCentral
				 // 
				 this->rbCentral->AutoSize = true;
				 this->rbCentral->Location = System::Drawing::Point(3, 26);
				 this->rbCentral->Name = L"rbCentral";
				 this->rbCentral->Size = System::Drawing::Size(92, 17);
				 this->rbCentral->TabIndex = 1;
				 this->rbCentral->Text = L"Центральная";
				 this->rbCentral->UseVisualStyleBackColor = true;
				 this->rbCentral->CheckedChanged += gcnew System::EventHandler(this, &MyForm::rbParallel_CheckedChanged);
				 // 
				 // flpZ0Settings
				 // 
				 this->flpZ0Settings->AutoSize = true;
				 this->flpZ0Settings->Controls->Add(this->lblZ0Settings);
				 this->flpZ0Settings->Controls->Add(this->tbZ0Settings);
				 this->flpZ0Settings->Enabled = false;
				 this->flpZ0Settings->Location = System::Drawing::Point(3, 49);
				 this->flpZ0Settings->Name = L"flpZ0Settings";
				 this->flpZ0Settings->Size = System::Drawing::Size(133, 26);
				 this->flpZ0Settings->TabIndex = 4;
				 // 
				 // lblZ0Settings
				 // 
				 this->lblZ0Settings->Anchor = System::Windows::Forms::AnchorStyles::None;
				 this->lblZ0Settings->AutoSize = true;
				 this->lblZ0Settings->Location = System::Drawing::Point(3, 6);
				 this->lblZ0Settings->Name = L"lblZ0Settings";
				 this->lblZ0Settings->Size = System::Drawing::Size(21, 13);
				 this->lblZ0Settings->TabIndex = 2;
				 this->lblZ0Settings->Text = L"z0:";
				 // 
				 // tbZ0Settings
				 // 
				 this->tbZ0Settings->Anchor = System::Windows::Forms::AnchorStyles::None;
				 this->tbZ0Settings->Location = System::Drawing::Point(30, 3);
				 this->tbZ0Settings->Name = L"tbZ0Settings";
				 this->tbZ0Settings->Size = System::Drawing::Size(100, 20);
				 this->tbZ0Settings->TabIndex = 3;
				 this->tbZ0Settings->Text = L"1000";
				 this->tbZ0Settings->TextChanged += gcnew System::EventHandler(this, &MyForm::tbZ0Settings_TextChanged);
				 // 
				 // gbWheelMouse
				 // 
				 this->gbWheelMouse->Controls->Add(this->flpWheelMouse);
				 this->gbWheelMouse->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->gbWheelMouse->Location = System::Drawing::Point(317, 3);
				 this->gbWheelMouse->Name = L"gbWheelMouse";
				 this->gbWheelMouse->Size = System::Drawing::Size(151, 112);
				 this->gbWheelMouse->TabIndex = 2;
				 this->gbWheelMouse->TabStop = false;
				 this->gbWheelMouse->Text = L"Масштаб (колесико)";
				 // 
				 // flpWheelMouse
				 // 
				 this->flpWheelMouse->AutoSize = true;
				 this->flpWheelMouse->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
				 this->flpWheelMouse->Controls->Add(this->rbWheelMouseX);
				 this->flpWheelMouse->Controls->Add(this->rbWheelMouseY);
				 this->flpWheelMouse->Controls->Add(this->rbWheelMouseZ);
				 this->flpWheelMouse->Controls->Add(this->rbWheelMouseXYZ);
				 this->flpWheelMouse->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->flpWheelMouse->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
				 this->flpWheelMouse->Location = System::Drawing::Point(3, 16);
				 this->flpWheelMouse->Name = L"flpWheelMouse";
				 this->flpWheelMouse->Size = System::Drawing::Size(145, 93);
				 this->flpWheelMouse->TabIndex = 3;
				 // 
				 // rbWheelMouseX
				 // 
				 this->rbWheelMouseX->AutoSize = true;
				 this->rbWheelMouseX->Location = System::Drawing::Point(3, 3);
				 this->rbWheelMouseX->Name = L"rbWheelMouseX";
				 this->rbWheelMouseX->Size = System::Drawing::Size(53, 17);
				 this->rbWheelMouseX->TabIndex = 0;
				 this->rbWheelMouseX->TabStop = true;
				 this->rbWheelMouseX->Text = L"ось X";
				 this->rbWheelMouseX->UseVisualStyleBackColor = true;
				 this->rbWheelMouseX->CheckedChanged += gcnew System::EventHandler(this, &MyForm::scaleAxis_CheckedChanged);
				 // 
				 // rbWheelMouseY
				 // 
				 this->rbWheelMouseY->AutoSize = true;
				 this->rbWheelMouseY->Location = System::Drawing::Point(3, 26);
				 this->rbWheelMouseY->Name = L"rbWheelMouseY";
				 this->rbWheelMouseY->Size = System::Drawing::Size(53, 17);
				 this->rbWheelMouseY->TabIndex = 1;
				 this->rbWheelMouseY->Text = L"ось Y";
				 this->rbWheelMouseY->UseVisualStyleBackColor = true;
				 this->rbWheelMouseY->CheckedChanged += gcnew System::EventHandler(this, &MyForm::scaleAxis_CheckedChanged);
				 // 
				 // rbWheelMouseZ
				 // 
				 this->rbWheelMouseZ->AutoSize = true;
				 this->rbWheelMouseZ->Location = System::Drawing::Point(3, 49);
				 this->rbWheelMouseZ->Name = L"rbWheelMouseZ";
				 this->rbWheelMouseZ->Size = System::Drawing::Size(53, 17);
				 this->rbWheelMouseZ->TabIndex = 2;
				 this->rbWheelMouseZ->Text = L"ось Z";
				 this->rbWheelMouseZ->UseVisualStyleBackColor = true;
				 this->rbWheelMouseZ->CheckedChanged += gcnew System::EventHandler(this, &MyForm::scaleAxis_CheckedChanged);
				 // 
				 // rbWheelMouseXYZ
				 // 
				 this->rbWheelMouseXYZ->AutoSize = true;
				 this->rbWheelMouseXYZ->Checked = true;
				 this->rbWheelMouseXYZ->Location = System::Drawing::Point(3, 72);
				 this->rbWheelMouseXYZ->Name = L"rbWheelMouseXYZ";
				 this->rbWheelMouseXYZ->Size = System::Drawing::Size(65, 17);
				 this->rbWheelMouseXYZ->TabIndex = 3;
				 this->rbWheelMouseXYZ->TabStop = true;
				 this->rbWheelMouseXYZ->Text = L"Все оси";
				 this->rbWheelMouseXYZ->UseVisualStyleBackColor = true;
				 this->rbWheelMouseXYZ->CheckedChanged += gcnew System::EventHandler(this, &MyForm::scaleAxis_CheckedChanged);
				 // 
				 // gbRightMouse
				 // 
				 this->gbRightMouse->Controls->Add(this->flpRightMouse);
				 this->gbRightMouse->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->gbRightMouse->Location = System::Drawing::Point(160, 3);
				 this->gbRightMouse->Name = L"gbRightMouse";
				 this->gbRightMouse->Size = System::Drawing::Size(151, 112);
				 this->gbRightMouse->TabIndex = 1;
				 this->gbRightMouse->TabStop = false;
				 this->gbRightMouse->Text = L"Поворот (правая кнопка)";
				 // 
				 // flpRightMouse
				 // 
				 this->flpRightMouse->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
					 | System::Windows::Forms::AnchorStyles::Left)
					 | System::Windows::Forms::AnchorStyles::Right));
				 this->flpRightMouse->AutoSize = true;
				 this->flpRightMouse->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
				 this->flpRightMouse->Controls->Add(this->rbRightMouseXY);
				 this->flpRightMouse->Controls->Add(this->rbRightMouseXZ);
				 this->flpRightMouse->Controls->Add(this->rbRightMouseYZ);
				 this->flpRightMouse->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
				 this->flpRightMouse->Location = System::Drawing::Point(3, 16);
				 this->flpRightMouse->Name = L"flpRightMouse";
				 this->flpRightMouse->Size = System::Drawing::Size(101, 69);
				 this->flpRightMouse->TabIndex = 2;
				 // 
				 // rbRightMouseXY
				 // 
				 this->rbRightMouseXY->AutoSize = true;
				 this->rbRightMouseXY->Checked = true;
				 this->rbRightMouseXY->Location = System::Drawing::Point(3, 3);
				 this->rbRightMouseXY->Name = L"rbRightMouseXY";
				 this->rbRightMouseXY->Size = System::Drawing::Size(95, 17);
				 this->rbRightMouseXY->TabIndex = 0;
				 this->rbRightMouseXY->TabStop = true;
				 this->rbRightMouseXY->Text = L"плоскость XY";
				 this->rbRightMouseXY->UseVisualStyleBackColor = true;
				 this->rbRightMouseXY->CheckedChanged += gcnew System::EventHandler(this, &MyForm::rotationPlane_CheckedChanged);
				 // 
				 // rbRightMouseXZ
				 // 
				 this->rbRightMouseXZ->AutoSize = true;
				 this->rbRightMouseXZ->Location = System::Drawing::Point(3, 26);
				 this->rbRightMouseXZ->Name = L"rbRightMouseXZ";
				 this->rbRightMouseXZ->Size = System::Drawing::Size(95, 17);
				 this->rbRightMouseXZ->TabIndex = 1;
				 this->rbRightMouseXZ->Text = L"плоскость XZ";
				 this->rbRightMouseXZ->UseVisualStyleBackColor = true;
				 this->rbRightMouseXZ->CheckedChanged += gcnew System::EventHandler(this, &MyForm::rotationPlane_CheckedChanged);
				 // 
				 // rbRightMouseYZ
				 // 
				 this->rbRightMouseYZ->AutoSize = true;
				 this->rbRightMouseYZ->Location = System::Drawing::Point(3, 49);
				 this->rbRightMouseYZ->Name = L"rbRightMouseYZ";
				 this->rbRightMouseYZ->Size = System::Drawing::Size(95, 17);
				 this->rbRightMouseYZ->TabIndex = 2;
				 this->rbRightMouseYZ->Text = L"плоскость YZ";
				 this->rbRightMouseYZ->UseVisualStyleBackColor = true;
				 this->rbRightMouseYZ->CheckedChanged += gcnew System::EventHandler(this, &MyForm::rotationPlane_CheckedChanged);
				 // 
				 // gbLeftMouse
				 // 
				 this->gbLeftMouse->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
					 | System::Windows::Forms::AnchorStyles::Left)
					 | System::Windows::Forms::AnchorStyles::Right));
				 this->gbLeftMouse->AutoSize = true;
				 this->gbLeftMouse->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
				 this->gbLeftMouse->Controls->Add(this->flpLeftMouse);
				 this->gbLeftMouse->Location = System::Drawing::Point(3, 3);
				 this->gbLeftMouse->Name = L"gbLeftMouse";
				 this->gbLeftMouse->Size = System::Drawing::Size(151, 112);
				 this->gbLeftMouse->TabIndex = 0;
				 this->gbLeftMouse->TabStop = false;
				 this->gbLeftMouse->Text = L"Смещение (левая кнопка)";
				 // 
				 // flpLeftMouse
				 // 
				 this->flpLeftMouse->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
					 | System::Windows::Forms::AnchorStyles::Left)
					 | System::Windows::Forms::AnchorStyles::Right));
				 this->flpLeftMouse->AutoSize = true;
				 this->flpLeftMouse->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
				 this->flpLeftMouse->Controls->Add(this->rbLeftMouseXY);
				 this->flpLeftMouse->Controls->Add(this->rbLeftMouseXZ);
				 this->flpLeftMouse->Controls->Add(this->rbLeftMouseYZ);
				 this->flpLeftMouse->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
				 this->flpLeftMouse->Location = System::Drawing::Point(3, 16);
				 this->flpLeftMouse->Name = L"flpLeftMouse";
				 this->flpLeftMouse->Size = System::Drawing::Size(101, 69);
				 this->flpLeftMouse->TabIndex = 1;
				 this->flpLeftMouse->WrapContents = false;
				 // 
				 // rbLeftMouseXY
				 // 
				 this->rbLeftMouseXY->AutoSize = true;
				 this->rbLeftMouseXY->Checked = true;
				 this->rbLeftMouseXY->Location = System::Drawing::Point(3, 3);
				 this->rbLeftMouseXY->Name = L"rbLeftMouseXY";
				 this->rbLeftMouseXY->Size = System::Drawing::Size(95, 17);
				 this->rbLeftMouseXY->TabIndex = 0;
				 this->rbLeftMouseXY->TabStop = true;
				 this->rbLeftMouseXY->Text = L"плоскость XY";
				 this->rbLeftMouseXY->UseVisualStyleBackColor = true;
				 this->rbLeftMouseXY->CheckedChanged += gcnew System::EventHandler(this, &MyForm::translatePlane_CheckedChanged);
				 // 
				 // rbLeftMouseXZ
				 // 
				 this->rbLeftMouseXZ->AutoSize = true;
				 this->rbLeftMouseXZ->Location = System::Drawing::Point(3, 26);
				 this->rbLeftMouseXZ->Name = L"rbLeftMouseXZ";
				 this->rbLeftMouseXZ->Size = System::Drawing::Size(95, 17);
				 this->rbLeftMouseXZ->TabIndex = 1;
				 this->rbLeftMouseXZ->Text = L"плоскость XZ";
				 this->rbLeftMouseXZ->UseVisualStyleBackColor = true;
				 this->rbLeftMouseXZ->CheckedChanged += gcnew System::EventHandler(this, &MyForm::translatePlane_CheckedChanged);
				 // 
				 // rbLeftMouseYZ
				 // 
				 this->rbLeftMouseYZ->AutoSize = true;
				 this->rbLeftMouseYZ->Location = System::Drawing::Point(3, 49);
				 this->rbLeftMouseYZ->Name = L"rbLeftMouseYZ";
				 this->rbLeftMouseYZ->Size = System::Drawing::Size(95, 17);
				 this->rbLeftMouseYZ->TabIndex = 2;
				 this->rbLeftMouseYZ->Text = L"плоскость YZ";
				 this->rbLeftMouseYZ->UseVisualStyleBackColor = true;
				 this->rbLeftMouseYZ->CheckedChanged += gcnew System::EventHandler(this, &MyForm::translatePlane_CheckedChanged);
				 // 
				 // tabFigure
				 // 
				 this->tabFigure->Controls->Add(this->tableLayoutFigureSettings);
				 this->tabFigure->Location = System::Drawing::Point(4, 22);
				 this->tabFigure->Name = L"tabFigure";
				 this->tabFigure->Padding = System::Windows::Forms::Padding(3);
				 this->tabFigure->Size = System::Drawing::Size(637, 124);
				 this->tabFigure->TabIndex = 1;
				 this->tabFigure->Text = L"Фигура";
				 this->tabFigure->UseVisualStyleBackColor = true;
				 // 
				 // tableLayoutFigureSettings
				 // 
				 this->tableLayoutFigureSettings->ColumnCount = 2;
				 this->tableLayoutFigureSettings->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
					 50)));
				 this->tableLayoutFigureSettings->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
					 50)));
				 this->tableLayoutFigureSettings->Controls->Add(this->gbFigureView, 0, 0);
				 this->tableLayoutFigureSettings->Controls->Add(this->gbSelectFigure, 0, 0);
				 this->tableLayoutFigureSettings->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->tableLayoutFigureSettings->Location = System::Drawing::Point(3, 3);
				 this->tableLayoutFigureSettings->Name = L"tableLayoutFigureSettings";
				 this->tableLayoutFigureSettings->RowCount = 1;
				 this->tableLayoutFigureSettings->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent,
					 50)));
				 this->tableLayoutFigureSettings->Size = System::Drawing::Size(631, 118);
				 this->tableLayoutFigureSettings->TabIndex = 0;
				 // 
				 // gbFigureView
				 // 
				 this->gbFigureView->Controls->Add(this->flpFigureView);
				 this->gbFigureView->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->gbFigureView->Location = System::Drawing::Point(318, 3);
				 this->gbFigureView->Name = L"gbFigureView";
				 this->gbFigureView->Size = System::Drawing::Size(310, 112);
				 this->gbFigureView->TabIndex = 1;
				 this->gbFigureView->TabStop = false;
				 this->gbFigureView->Text = L"Вид";
				 // 
				 // flpFigureView
				 // 
				 this->flpFigureView->AutoSize = true;
				 this->flpFigureView->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
				 this->flpFigureView->Controls->Add(this->rbFrontFigureView);
				 this->flpFigureView->Controls->Add(this->rbTopFigureView);
				 this->flpFigureView->Controls->Add(this->rbSideFigureView);
				 this->flpFigureView->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->flpFigureView->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
				 this->flpFigureView->Location = System::Drawing::Point(3, 16);
				 this->flpFigureView->Name = L"flpFigureView";
				 this->flpFigureView->Size = System::Drawing::Size(304, 93);
				 this->flpFigureView->TabIndex = 2;
				 this->flpFigureView->WrapContents = false;
				 // 
				 // rbFrontFigureView
				 // 
				 this->rbFrontFigureView->AutoSize = true;
				 this->rbFrontFigureView->Checked = true;
				 this->rbFrontFigureView->Location = System::Drawing::Point(3, 3);
				 this->rbFrontFigureView->Name = L"rbFrontFigureView";
				 this->rbFrontFigureView->Size = System::Drawing::Size(117, 17);
				 this->rbFrontFigureView->TabIndex = 0;
				 this->rbFrontFigureView->TabStop = true;
				 this->rbFrontFigureView->Text = L"Спереди (вдоль Z)";
				 this->rbFrontFigureView->UseVisualStyleBackColor = true;
				 this->rbFrontFigureView->CheckedChanged += gcnew System::EventHandler(this, &MyForm::rbFrontFigureView_CheckedChanged);
				 // 
				 // rbTopFigureView
				 // 
				 this->rbTopFigureView->AutoSize = true;
				 this->rbTopFigureView->Location = System::Drawing::Point(3, 26);
				 this->rbTopFigureView->Name = L"rbTopFigureView";
				 this->rbTopFigureView->Size = System::Drawing::Size(109, 17);
				 this->rbTopFigureView->TabIndex = 1;
				 this->rbTopFigureView->Text = L"Сверху (вдоль Y)";
				 this->rbTopFigureView->UseVisualStyleBackColor = true;
				 this->rbTopFigureView->CheckedChanged += gcnew System::EventHandler(this, &MyForm::rbFrontFigureView_CheckedChanged);
				 // 
				 // rbSideFigureView
				 // 
				 this->rbSideFigureView->AutoSize = true;
				 this->rbSideFigureView->Location = System::Drawing::Point(3, 49);
				 this->rbSideFigureView->Name = L"rbSideFigureView";
				 this->rbSideFigureView->Size = System::Drawing::Size(104, 17);
				 this->rbSideFigureView->TabIndex = 2;
				 this->rbSideFigureView->Text = L"Сбоку (вдоль X)";
				 this->rbSideFigureView->UseVisualStyleBackColor = true;
				 this->rbSideFigureView->CheckedChanged += gcnew System::EventHandler(this, &MyForm::rbFrontFigureView_CheckedChanged);
				 // 
				 // gbSelectFigure
				 // 
				 this->gbSelectFigure->Controls->Add(this->flowLayoutPanel1);
				 this->gbSelectFigure->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->gbSelectFigure->Location = System::Drawing::Point(3, 3);
				 this->gbSelectFigure->Name = L"gbSelectFigure";
				 this->gbSelectFigure->Size = System::Drawing::Size(309, 112);
				 this->gbSelectFigure->TabIndex = 0;
				 this->gbSelectFigure->TabStop = false;
				 this->gbSelectFigure->Text = L"Фигура";
				 // 
				 // flowLayoutPanel1
				 // 
				 this->flowLayoutPanel1->AutoSize = true;
				 this->flowLayoutPanel1->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
				 this->flowLayoutPanel1->Controls->Add(this->comboBoxFigure);
				 this->flowLayoutPanel1->Controls->Add(this->btnResetFigure);
				 this->flowLayoutPanel1->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
				 this->flowLayoutPanel1->Location = System::Drawing::Point(90, 26);
				 this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
				 this->flowLayoutPanel1->Size = System::Drawing::Size(146, 59);
				 this->flowLayoutPanel1->TabIndex = 2;
				 // 
				 // comboBoxFigure
				 // 
				 this->comboBoxFigure->Anchor = System::Windows::Forms::AnchorStyles::None;
				 this->comboBoxFigure->FormattingEnabled = true;
				 this->comboBoxFigure->Items->AddRange(gcnew cli::array< System::Object^  >(7) {
					 L"Тэтраэдр", L"Октаэдр", L"Икосаэдр", L"Сфера",
						 L"Тор", L"Капля", L"Пружина"
				 });
				 this->comboBoxFigure->Location = System::Drawing::Point(23, 3);
				 this->comboBoxFigure->MinimumSize = System::Drawing::Size(100, 0);
				 this->comboBoxFigure->Name = L"comboBoxFigure";
				 this->comboBoxFigure->Size = System::Drawing::Size(100, 21);
				 this->comboBoxFigure->TabIndex = 0;
				 this->comboBoxFigure->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBoxFigure_SelectedIndexChanged);
				 // 
				 // btnResetFigure
				 // 
				 this->btnResetFigure->Anchor = System::Windows::Forms::AnchorStyles::None;
				 this->btnResetFigure->Location = System::Drawing::Point(3, 30);
				 this->btnResetFigure->Name = L"btnResetFigure";
				 this->btnResetFigure->Size = System::Drawing::Size(140, 26);
				 this->btnResetFigure->TabIndex = 1;
				 this->btnResetFigure->Text = L"Начальное состояние";
				 this->btnResetFigure->UseVisualStyleBackColor = true;
				 this->btnResetFigure->Click += gcnew System::EventHandler(this, &MyForm::btnResetFigure_Click);
				 // 
				 // tabRotation
				 // 
				 this->tabRotation->Controls->Add(this->tableLayoutRotationSettings);
				 this->tabRotation->Location = System::Drawing::Point(4, 22);
				 this->tabRotation->Name = L"tabRotation";
				 this->tabRotation->Padding = System::Windows::Forms::Padding(3);
				 this->tabRotation->Size = System::Drawing::Size(637, 124);
				 this->tabRotation->TabIndex = 2;
				 this->tabRotation->Text = L"Вращение";
				 this->tabRotation->UseVisualStyleBackColor = true;
				 // 
				 // tableLayoutRotationSettings
				 // 
				 this->tableLayoutRotationSettings->ColumnCount = 2;
				 this->tableLayoutRotationSettings->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
					 75)));
				 this->tableLayoutRotationSettings->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
					 25)));
				 this->tableLayoutRotationSettings->Controls->Add(this->gbCustomLineRotation, 0, 0);
				 this->tableLayoutRotationSettings->Controls->Add(this->lblForStartCustomLineRotation, 1, 0);
				 this->tableLayoutRotationSettings->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->tableLayoutRotationSettings->Location = System::Drawing::Point(3, 3);
				 this->tableLayoutRotationSettings->Name = L"tableLayoutRotationSettings";
				 this->tableLayoutRotationSettings->RowCount = 1;
				 this->tableLayoutRotationSettings->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent,
					 100)));
				 this->tableLayoutRotationSettings->Size = System::Drawing::Size(631, 118);
				 this->tableLayoutRotationSettings->TabIndex = 1;
				 // 
				 // gbCustomLineRotation
				 // 
				 this->gbCustomLineRotation->Controls->Add(this->tableLayoutCustomLineRotation);
				 this->gbCustomLineRotation->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->gbCustomLineRotation->Location = System::Drawing::Point(0, 0);
				 this->gbCustomLineRotation->Margin = System::Windows::Forms::Padding(0);
				 this->gbCustomLineRotation->Name = L"gbCustomLineRotation";
				 this->gbCustomLineRotation->Size = System::Drawing::Size(473, 118);
				 this->gbCustomLineRotation->TabIndex = 0;
				 this->gbCustomLineRotation->TabStop = false;
				 this->gbCustomLineRotation->Text = L"Произвольная прямая";
				 // 
				 // tableLayoutCustomLineRotation
				 // 
				 this->tableLayoutCustomLineRotation->ColumnCount = 2;
				 this->tableLayoutCustomLineRotation->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
					 50)));
				 this->tableLayoutCustomLineRotation->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
					 50)));
				 this->tableLayoutCustomLineRotation->Controls->Add(this->tbAnimationDelay, 1, 2);
				 this->tableLayoutCustomLineRotation->Controls->Add(this->lblForAnimationDelay, 0, 2);
				 this->tableLayoutCustomLineRotation->Controls->Add(this->lblForCustomVectorRotation, 0, 1);
				 this->tableLayoutCustomLineRotation->Controls->Add(this->tbCustomPointRotation, 1, 0);
				 this->tableLayoutCustomLineRotation->Controls->Add(this->tbCustomVectorRotation, 1, 1);
				 this->tableLayoutCustomLineRotation->Controls->Add(this->lblForCustomPointRotation, 0, 0);
				 this->tableLayoutCustomLineRotation->Controls->Add(this->btnStartCustomLineRotation, 0, 3);
				 this->tableLayoutCustomLineRotation->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->tableLayoutCustomLineRotation->Location = System::Drawing::Point(3, 16);
				 this->tableLayoutCustomLineRotation->Margin = System::Windows::Forms::Padding(0);
				 this->tableLayoutCustomLineRotation->Name = L"tableLayoutCustomLineRotation";
				 this->tableLayoutCustomLineRotation->RowCount = 4;
				 this->tableLayoutCustomLineRotation->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent,
					 33.33333F)));
				 this->tableLayoutCustomLineRotation->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent,
					 33.33333F)));
				 this->tableLayoutCustomLineRotation->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent,
					 33.33333F)));
				 this->tableLayoutCustomLineRotation->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
					 32)));
				 this->tableLayoutCustomLineRotation->Size = System::Drawing::Size(467, 99);
				 this->tableLayoutCustomLineRotation->TabIndex = 0;
				 // 
				 // tbAnimationDelay
				 // 
				 this->tbAnimationDelay->Anchor = System::Windows::Forms::AnchorStyles::Left;
				 this->tbAnimationDelay->Location = System::Drawing::Point(236, 47);
				 this->tbAnimationDelay->Name = L"tbAnimationDelay";
				 this->tbAnimationDelay->Size = System::Drawing::Size(100, 20);
				 this->tbAnimationDelay->TabIndex = 6;
				 this->tbAnimationDelay->Text = L"80";
				 // 
				 // lblForAnimationDelay
				 // 
				 this->lblForAnimationDelay->Anchor = System::Windows::Forms::AnchorStyles::None;
				 this->lblForAnimationDelay->AutoSize = true;
				 this->lblForAnimationDelay->Location = System::Drawing::Point(74, 48);
				 this->lblForAnimationDelay->Name = L"lblForAnimationDelay";
				 this->lblForAnimationDelay->Size = System::Drawing::Size(84, 13);
				 this->lblForAnimationDelay->TabIndex = 5;
				 this->lblForAnimationDelay->Text = L"Задержка (мс):";
				 // 
				 // lblForCustomVectorRotation
				 // 
				 this->lblForCustomVectorRotation->Anchor = System::Windows::Forms::AnchorStyles::None;
				 this->lblForCustomVectorRotation->AutoSize = true;
				 this->lblForCustomVectorRotation->Location = System::Drawing::Point(8, 26);
				 this->lblForCustomVectorRotation->Name = L"lblForCustomVectorRotation";
				 this->lblForCustomVectorRotation->Size = System::Drawing::Size(217, 13);
				 this->lblForCustomVectorRotation->TabIndex = 3;
				 this->lblForCustomVectorRotation->Text = L"Коэффициенты направляющего вектора:";
				 // 
				 // tbCustomPointRotation
				 // 
				 this->tbCustomPointRotation->Anchor = System::Windows::Forms::AnchorStyles::Left;
				 this->tbCustomPointRotation->Location = System::Drawing::Point(236, 3);
				 this->tbCustomPointRotation->Name = L"tbCustomPointRotation";
				 this->tbCustomPointRotation->Size = System::Drawing::Size(100, 20);
				 this->tbCustomPointRotation->TabIndex = 0;
				 this->tbCustomPointRotation->Text = L"1,2,3";
				 // 
				 // tbCustomVectorRotation
				 // 
				 this->tbCustomVectorRotation->Anchor = System::Windows::Forms::AnchorStyles::Left;
				 this->tbCustomVectorRotation->Location = System::Drawing::Point(236, 25);
				 this->tbCustomVectorRotation->Name = L"tbCustomVectorRotation";
				 this->tbCustomVectorRotation->Size = System::Drawing::Size(100, 20);
				 this->tbCustomVectorRotation->TabIndex = 1;
				 this->tbCustomVectorRotation->Text = L"0.1,0.3,0.4";
				 // 
				 // lblForCustomPointRotation
				 // 
				 this->lblForCustomPointRotation->Anchor = System::Windows::Forms::AnchorStyles::None;
				 this->lblForCustomPointRotation->AutoSize = true;
				 this->lblForCustomPointRotation->Location = System::Drawing::Point(65, 4);
				 this->lblForCustomPointRotation->Name = L"lblForCustomPointRotation";
				 this->lblForCustomPointRotation->Size = System::Drawing::Size(103, 13);
				 this->lblForCustomPointRotation->TabIndex = 2;
				 this->lblForCustomPointRotation->Text = L"Координаты точки:";
				 // 
				 // btnStartCustomLineRotation
				 // 
				 this->tableLayoutCustomLineRotation->SetColumnSpan(this->btnStartCustomLineRotation, 2);
				 this->btnStartCustomLineRotation->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->btnStartCustomLineRotation->Location = System::Drawing::Point(3, 69);
				 this->btnStartCustomLineRotation->Name = L"btnStartCustomLineRotation";
				 this->btnStartCustomLineRotation->Size = System::Drawing::Size(461, 27);
				 this->btnStartCustomLineRotation->TabIndex = 4;
				 this->btnStartCustomLineRotation->Text = L"Анимация вращения";
				 this->btnStartCustomLineRotation->UseVisualStyleBackColor = true;
				 this->btnStartCustomLineRotation->Click += gcnew System::EventHandler(this, &MyForm::btnStartCustomLineRotation_Click);
				 // 
				 // lblForStartCustomLineRotation
				 // 
				 this->lblForStartCustomLineRotation->AutoSize = true;
				 this->lblForStartCustomLineRotation->Location = System::Drawing::Point(476, 0);
				 this->lblForStartCustomLineRotation->Name = L"lblForStartCustomLineRotation";
				 this->lblForStartCustomLineRotation->Size = System::Drawing::Size(151, 117);
				 this->lblForStartCustomLineRotation->TabIndex = 1;
				 this->lblForStartCustomLineRotation->Text = L"Не применяйте трансформации к фигуре во время анимации во избежание сбоев матрицы"
					 L".  \r\nЕсли во время вращения появляются артефакты, увеличьте параметр \"Задержка\"."
					 L"\r\n";
				 // 
				 // rootTableLayoutPanel
				 // 
				 this->rootTableLayoutPanel->ColumnCount = 1;
				 this->rootTableLayoutPanel->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
					 100)));
				 this->rootTableLayoutPanel->Controls->Add(this->pictureBox1, 0, 1);
				 this->rootTableLayoutPanel->Controls->Add(this->mainTabControl, 0, 0);
				 this->rootTableLayoutPanel->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->rootTableLayoutPanel->Location = System::Drawing::Point(0, 0);
				 this->rootTableLayoutPanel->Name = L"rootTableLayoutPanel";
				 this->rootTableLayoutPanel->RowCount = 2;
				 this->rootTableLayoutPanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute,
					 150)));
				 this->rootTableLayoutPanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
				 this->rootTableLayoutPanel->Size = System::Drawing::Size(651, 532);
				 this->rootTableLayoutPanel->TabIndex = 2;
				 // 
				 // pictureBox1
				 // 
				 this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->pictureBox1->Location = System::Drawing::Point(3, 153);
				 this->pictureBox1->Name = L"pictureBox1";
				 this->pictureBox1->Size = System::Drawing::Size(645, 376);
				 this->pictureBox1->TabIndex = 3;
				 this->pictureBox1->TabStop = false;
				 this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBox1_MouseDown);
				 this->pictureBox1->MouseEnter += gcnew System::EventHandler(this, &MyForm::pictureBox1_MouseEnter);
				 this->pictureBox1->MouseLeave += gcnew System::EventHandler(this, &MyForm::pictureBox1_MouseLeave);
				 this->pictureBox1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBox1_MouseMove);
				 this->pictureBox1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBox1_MouseUp);
				 this->pictureBox1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBox1_MouseWheel);
				 // 
				 // bgWorkerRotation
				 // 
				 this->bgWorkerRotation->WorkerReportsProgress = true;
				 this->bgWorkerRotation->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MyForm::bgWorkerRotation_DoWork);
				 this->bgWorkerRotation->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &MyForm::bgWorkerRotation_ProgressChanged);
				 this->bgWorkerRotation->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &MyForm::bgWorkerRotation_RunWorkerCompleted);
				 // 
				 // tabAboutMe
				 // 
				 this->tabAboutMe->Controls->Add(this->lblAboutMe);
				 this->tabAboutMe->Location = System::Drawing::Point(4, 22);
				 this->tabAboutMe->Name = L"tabAboutMe";
				 this->tabAboutMe->Padding = System::Windows::Forms::Padding(3);
				 this->tabAboutMe->Size = System::Drawing::Size(637, 124);
				 this->tabAboutMe->TabIndex = 3;
				 this->tabAboutMe->Text = L"О программе";
				 this->tabAboutMe->UseVisualStyleBackColor = true;
				 // 
				 // lblAboutMe
				 // 
				 this->lblAboutMe->AutoSize = true;
				 this->lblAboutMe->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->lblAboutMe->Location = System::Drawing::Point(3, 3);
				 this->lblAboutMe->Name = L"lblAboutMe";
				 this->lblAboutMe->Size = System::Drawing::Size(619, 104);
				 this->lblAboutMe->TabIndex = 0;
				 this->lblAboutMe->Text = resources->GetString(L"lblAboutMe.Text");
				 // 
				 // MyForm
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(651, 532);
				 this->Controls->Add(this->rootTableLayoutPanel);
				 this->Name = L"MyForm";
				 this->Text = L"Семинар 5, Куликов Андрей (БПИ 121)";
				 this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
				 this->Shown += gcnew System::EventHandler(this, &MyForm::MyForm_Shown);
				 this->Resize += gcnew System::EventHandler(this, &MyForm::MyForm_Resize);
				 this->mainTabControl->ResumeLayout(false);
				 this->tabMouseSettings->ResumeLayout(false);
				 this->tableLayoutMouseSettings->ResumeLayout(false);
				 this->tableLayoutMouseSettings->PerformLayout();
				 this->gbProjectionSettings->ResumeLayout(false);
				 this->gbProjectionSettings->PerformLayout();
				 this->flpProjectionSettings->ResumeLayout(false);
				 this->flpProjectionSettings->PerformLayout();
				 this->flpZ0Settings->ResumeLayout(false);
				 this->flpZ0Settings->PerformLayout();
				 this->gbWheelMouse->ResumeLayout(false);
				 this->gbWheelMouse->PerformLayout();
				 this->flpWheelMouse->ResumeLayout(false);
				 this->flpWheelMouse->PerformLayout();
				 this->gbRightMouse->ResumeLayout(false);
				 this->gbRightMouse->PerformLayout();
				 this->flpRightMouse->ResumeLayout(false);
				 this->flpRightMouse->PerformLayout();
				 this->gbLeftMouse->ResumeLayout(false);
				 this->gbLeftMouse->PerformLayout();
				 this->flpLeftMouse->ResumeLayout(false);
				 this->flpLeftMouse->PerformLayout();
				 this->tabFigure->ResumeLayout(false);
				 this->tableLayoutFigureSettings->ResumeLayout(false);
				 this->gbFigureView->ResumeLayout(false);
				 this->gbFigureView->PerformLayout();
				 this->flpFigureView->ResumeLayout(false);
				 this->flpFigureView->PerformLayout();
				 this->gbSelectFigure->ResumeLayout(false);
				 this->gbSelectFigure->PerformLayout();
				 this->flowLayoutPanel1->ResumeLayout(false);
				 this->tabRotation->ResumeLayout(false);
				 this->tableLayoutRotationSettings->ResumeLayout(false);
				 this->tableLayoutRotationSettings->PerformLayout();
				 this->gbCustomLineRotation->ResumeLayout(false);
				 this->tableLayoutCustomLineRotation->ResumeLayout(false);
				 this->tableLayoutCustomLineRotation->PerformLayout();
				 this->rootTableLayoutPanel->ResumeLayout(false);
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
				 this->tabAboutMe->ResumeLayout(false);
				 this->tabAboutMe->PerformLayout();
				 this->ResumeLayout(false);

			 }
#pragma endregion

#pragma region Event Hadlers
			 void keepTranslationCoords(array<int>^ arr, System::Windows::Forms::MouseEventArgs^  e)
			 {
				 switch (translationPlane)
				 {
				 case TranslationPlane::XY:
					 arr[0] = e->Location.X;
					 arr[1] = e->Location.Y;
					 break;
				 case TranslationPlane::XZ:
					 arr[0] = e->Location.X;
					 arr[2] = e->Location.Y;
					 break;
				 case TranslationPlane::YZ:
					 arr[1] = e->Location.X;
					 arr[2] = e->Location.Y;
					 break;
				 }
			 }

			 void keepRotationsCoords(array<int>^ arr, System::Windows::Forms::MouseEventArgs^  e)
			 {
				 switch (rotationPlane)
				 {
				 case RotationPlane::XY:
					 arr[0] = e->Location.Y;
					 arr[1] = e->Location.X;

					 break;
				 case RotationPlane::XZ:
					 arr[0] = e->Location.Y;
					 arr[2] = e->Location.X;

					 break;
				 case RotationPlane::YZ:
					 arr[1] = e->Location.X;
					 arr[2] = e->Location.Y;

					 break;
				 }
			 }



	private: System::Void pictureBox1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		if (e->Button == System::Windows::Forms::MouseButtons::Left) {
			UIState::leftBtnPressed = true;
			keepTranslationCoords(UIState::offsets, e);
		}
		if (e->Button == System::Windows::Forms::MouseButtons::Right) {
			UIState::rightBtnPressed = true;
			keepRotationsCoords(UIState::rotations, e);
		}
	}


	private: System::Void pictureBox1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		if (UIState::leftBtnPressed) {
			keepTranslationCoords(tempValues, e);
			GeomHelper::translate(GraphicsStorage::vertices,
				tempValues[0] - UIState::offsets[0],
				tempValues[1] - UIState::offsets[1],
				tempValues[2] - UIState::offsets[2]);
			Array::Copy(tempValues, 0, UIState::offsets, 0, 3);
		}

		if (UIState::rightBtnPressed)
		{
			keepRotationsCoords(tempValues, e);
			int deltaX = tempValues[0] - UIState::rotations[0];
			int deltaY = tempValues[1] - UIState::rotations[1];
			int deltaZ = tempValues[2] - UIState::rotations[2];
			if (deltaX != 0) GeomHelper::rotateX(GraphicsStorage::vertices, deltaX);
			if (deltaY != 0) GeomHelper::rotateY(GraphicsStorage::vertices, deltaY);
			if (deltaZ != 0) GeomHelper::rotateZ(GraphicsStorage::vertices, deltaZ);
			Array::Copy(tempValues, 0, UIState::rotations, 0, 3);
		}
		redraw(UIState::projection, UIState::dist);
	}
	private: System::Void pictureBox1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		UIState::leftBtnPressed = false;
		UIState::rightBtnPressed = false;
	}
	private: System::Void  pictureBox1_MouseWheel(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e)
	{

		double x = 1, y = 1, z = 1, all = 1;
		double value = e->Delta > 0 ? value = 1.1 : value = 1 / 1.1;

		switch (scaleAxis)
		{
		case ScaleAxis::X:
			x = value;
			break;
		case ScaleAxis::Y:
			y = value;
			break;
		case ScaleAxis::Z:
			z = value;
			break;
		case ScaleAxis::XYZ:
			all = 1 / value; //инвертированная шкала
			break;
		}
		GeomHelper::scale(GraphicsStorage::vertices, x, y, z, all);
		redraw(UIState::projection, UIState::dist);
	}
	private: System::Void translatePlane_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		if (sender == rbLeftMouseXY)
			translationPlane = TranslationPlane::XY;
		else if (sender == rbLeftMouseXZ)
			translationPlane = TranslationPlane::XZ;
		else if (sender == rbLeftMouseYZ)
			translationPlane = TranslationPlane::YZ;
	}

	private: System::Void rotationPlane_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		if (sender == rbRightMouseXY)
			rotationPlane = RotationPlane::XY;
		else if (sender == rbRightMouseXZ)
			rotationPlane = RotationPlane::XZ;
		else if (sender == rbRightMouseYZ)
			rotationPlane = RotationPlane::YZ;
	}
	private: System::Void MyForm_Shown(System::Object^  sender, System::EventArgs^  e) {
		onSizeChaged();
		comboBoxFigure->SelectedIndex = 2;
		redraw(UIState::projection, UIState::dist);
	}
	private: System::Void scaleAxis_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		if (sender == rbWheelMouseX)
			scaleAxis = ScaleAxis::X;
		else if (sender == rbWheelMouseY)
			scaleAxis = ScaleAxis::Y;
		else if (sender == rbWheelMouseZ)
			scaleAxis = ScaleAxis::Z;
		else if (sender == rbWheelMouseXYZ)
			scaleAxis = ScaleAxis::XYZ;
	}
	private: System::Void comboBoxFigure_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		//	L"Тэтраэдр", L"Октаэдр", L"Икосаэдр", L"Сфера",
		//L"Тор", L"Капля", L"Пружина"
		switch (((ComboBox^)sender)->SelectedIndex)
		{
		case 0: GraphicsStorage::setFigure(Tetrahedron::Instance); break;
		case 1: GraphicsStorage::setFigure(Octahedron::Instance); break;
		case 2: GraphicsStorage::setFigure(Icosahedron::Instance); break;
		case 3: GraphicsStorage::setFigure(Sphere::Instance); break;
		case 4: GraphicsStorage::setFigure(Torus::Instance); break;
		case 5: GraphicsStorage::setFigure(Drop::Instance); break;
		case 6: GraphicsStorage::setFigure(Spring::Instance); break;
		}
		redraw(UIState::projection, UIState::dist);
	}
	private: System::Void MyForm_Resize(System::Object^  sender, System::EventArgs^  e) {
		onSizeChaged();
	}
	private: System::Void pictureBox1_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
		pictureBox1->Focus();
	}
	private: System::Void pictureBox1_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
		this->Focus();
	}

	private:
		bool parsePointCoords(String^ value, array<double>^ coords)
		{
			array<String^>^ splitted = value->Split(',');
			if (splitted->Length != 3) return false;
			for (int i = 0; i < 3; i++)
			{
				//en-GB - чтобы дробная часть отделялась символом "."
				if (!Double::TryParse(splitted[i], NumberStyles::Number, CultureInfo::CreateSpecificCulture("en-GB"), coords[i]))
					return false;
			}
			return true;
		}
		bool parseVectorCoeff(String^ value, array<double>^ coef)
		{
			array<String^>^ splitted = value->Split(',');
			if (splitted->Length != 3) return false;
			for (int i = 0; i < 3; i++)
			{
				if (!Double::TryParse(splitted[i], NumberStyles::Number, CultureInfo::CreateSpecificCulture("en-GB"), coef[i]))
					return false;
			}
			return true;
		}

		bool parseDelayValue(String^ value, int% delay)
		{
			if (!int::TryParse(value, delay) || delay < 0)
				return false;
			return true;
		}

	private: System::Void btnStartCustomLineRotation_Click(System::Object^  sender, System::EventArgs^  e) {
		if (bgWorkerRotation->IsBusy) return;
		UIState::customPoint = gcnew array<double>(3);
		UIState::customVector = gcnew array<double>(3);

		bool ready = true;
		if (!parsePointCoords(tbCustomPointRotation->Text, UIState::customPoint))
		{
			ready = false;
			MessageBox::Show("Введите коэффициенты прямой через запятую без пробелов.\n Например: 1,2,3");
		}
		if (!parseVectorCoeff(tbCustomVectorRotation->Text, UIState::customVector))
		{
			ready = false;
			MessageBox::Show("Введите коэффициенты направляющего вектора через запятую без пробелов.\n Например: 4,5,6");
		}
		if (!parseDelayValue(tbAnimationDelay->Text, UIState::delay))
		{
			ready = false;
			MessageBox::Show("Введите время задержки анимации в мс");
		}
		//нормировка направляющего вектора
		double norm = Math::Sqrt(
			Math::Pow(UIState::customVector[0], 2) +
			Math::Pow(UIState::customVector[1], 2) +
			Math::Pow(UIState::customVector[2], 2));
		UIState::customVector[0] /= norm;
		UIState::customVector[1] /= norm;
		UIState::customVector[2] /= norm;

	
		if (ready) {
			bgWorkerRotation->RunWorkerAsync();
		}
	}
	private: System::Void bgWorkerRotation_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
		int points = 120;
		double step = 360 / (points - 1);
		//вращение вдоль оси, параллельной X
		bool onlyX = UIState::customVector[1] == 0 && UIState::customVector[2] == 0;

		for (int i = 0; i < points; i++)
		{
			GeomHelper::translate(GraphicsStorage::vertices,
				-UIState::customPoint[0], -UIState::customPoint[1],
				-UIState::customPoint[2]);
			if (onlyX)
			{
				GeomHelper::rotateX(GraphicsStorage::vertices, step);
			}
			else {
				GeomHelper::rotateX(GraphicsStorage::vertices,
					UIState::customVector[1], UIState::customVector[2], false);
				GeomHelper::rotateY(GraphicsStorage::vertices,
					UIState::customVector[0], UIState::customVector[1], UIState::customVector[2], false);
				GeomHelper::rotateZ(GraphicsStorage::vertices, step);
				GeomHelper::rotateY(GraphicsStorage::vertices,
					UIState::customVector[0], UIState::customVector[1], UIState::customVector[2], true);
				GeomHelper::rotateX(GraphicsStorage::vertices,
					UIState::customVector[1], UIState::customVector[2], true);
			}
			GeomHelper::translate(GraphicsStorage::vertices,
				UIState::customPoint[0], UIState::customPoint[1],
				UIState::customPoint[2]);
			bgWorkerRotation->ReportProgress(0);
			System::Threading::Thread::Sleep(UIState::delay);
		}

	}
	private: System::Void bgWorkerRotation_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) {
		redraw(UIState::projection, UIState::dist);
	}
	private: System::Void bgWorkerRotation_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) {
	}
	private: System::Void rbParallel_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		if (sender == rbParallel) {
			flpZ0Settings->Enabled = false;
			UIState::projection = GeomHelper::Projection::Parallel;
		}
		else {
			flpZ0Settings->Enabled = true;
			UIState::projection = GeomHelper::Projection::Central;
		}
		redraw(UIState::projection, UIState::dist);
	}
	private: System::Void tbZ0Settings_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		String^ text = ((TextBox^)sender)->Text;
		double result = 0;
		if (double::TryParse(text, NumberStyles::Number, CultureInfo::CreateSpecificCulture("en-GB"), result) && result != 0)
		{
			UIState::dist = result;
			redraw(UIState::projection, UIState::dist);
		}
	}
	private: System::Void rbFrontFigureView_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		if (sender == rbFrontFigureView)
			UIState::projection = GeomHelper::Projection::Front;
		else if (sender == rbTopFigureView)
			UIState::projection = GeomHelper::Projection::Top;
		else if (sender == rbSideFigureView)
			UIState::projection = GeomHelper::Projection::Side;
		redraw(UIState::projection, UIState::dist);
	}
	private: System::Void tbFigureViewDist_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		String^ text = ((TextBox^)sender)->Text;
		double result = 0;
		if (double::TryParse(text, NumberStyles::Number, CultureInfo::CreateSpecificCulture("en-GB"), result))
		{
			UIState::dist = result;
			redraw(UIState::projection, UIState::dist);
		}
	}
	private: System::Void btnResetFigure_Click(System::Object^  sender, System::EventArgs^  e) {
		GraphicsStorage::resetVertices();
		redraw(UIState::projection, UIState::dist);
	}
	};
#pragma endregion
}
