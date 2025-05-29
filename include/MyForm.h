#pragma once

#include "PromotePawnForm.h"

namespace SharokinChess {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->InitializeBoard();
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
	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ playToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ boardToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ levelToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ aboutToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ newToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ saveAsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^ exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ exitToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^ undoToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ redoToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^ thinkToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ setupToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ reverseToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ twoPlayesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem2;
	private: System::Windows::Forms::ToolStripMenuItem^ minutesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ aboutSharokinChessToolStripMenuItem;
	private: System::Windows::Forms::Panel^ panelRight;
	private: System::Windows::Forms::Panel^ panelMoveHistory;
	private: System::Windows::Forms::RichTextBox^ richtextMoves;
	private: System::Windows::Forms::MenuStrip^ menuStripMain;
	private: System::Windows::Forms::MenuStrip^ menuStripStop;

	private: System::Windows::Forms::ToolStripMenuItem^ gameToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ mnNew;

	private: System::Windows::Forms::ToolStripMenuItem^ mnOpen;
	private: System::Windows::Forms::ToolStripMenuItem^ mnSave;
	private: System::Windows::Forms::ToolStripMenuItem^ mnSaveAs;




	private: System::Windows::Forms::ToolStripMenuItem^ toolStripMenuItem3;
	private: System::Windows::Forms::ToolStripMenuItem^ toolStripMenuItem4;
	private: System::Windows::Forms::ToolStripMenuItem^ toolStripMenuItem5;
	private: System::Windows::Forms::ToolStripMenuItem^ toolStripMenuItem6;
	private: System::Windows::Forms::ToolStripMenuItem^ toolStripMenuItem8;
	private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem7;
	private: System::Windows::Forms::ToolStripMenuItem^ mnExit;
	private: System::Windows::Forms::ToolStripMenuItem^ mnStepBack;

	private: System::Windows::Forms::ToolStripMenuItem^ mnStepForward;




	private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem13;
	private: System::Windows::Forms::ToolStripMenuItem^ mnThink;

	private: System::Windows::Forms::ToolStripMenuItem^ mnSetup;

	private: System::Windows::Forms::ToolStripMenuItem^ mnReverse;
	private: System::Windows::Forms::Panel^ panelNormal;
	private: System::Windows::Forms::Panel^ panelSetup;
	private: System::Windows::Forms::Panel^ panelNormalTop;
	private: System::Windows::Forms::RichTextBox^ richTextMovesHistory;
	private: System::Windows::Forms::Button^ buttonSetupDone;


	private: System::Windows::Forms::Button^ buttonCancelSetup;
	private: System::Windows::Forms::Panel^ panelDiffPieces;
	private: System::Windows::Forms::Panel^ panelTimes;





	private: System::Windows::Forms::ToolStripMenuItem^ mnTwoPlayers;
	private: System::Windows::Forms::ToolStripSeparator^ toolStripMenuItem15;
	private: System::Windows::Forms::Panel^ panelSelectedPiece;










































private: System::Windows::Forms::Button^ buttonWhiteKing;
private: System::Windows::Forms::Button^ buttonBlackPawn;

private: System::Windows::Forms::Button^ buttonBlackKnight;

private: System::Windows::Forms::Button^ buttonBlackBishop;

private: System::Windows::Forms::Button^ buttonBlackRook;

private: System::Windows::Forms::Button^ buttonBlackQueen;

private: System::Windows::Forms::Button^ buttonBlackKing;

private: System::Windows::Forms::Button^ buttonWhitePawn;

private: System::Windows::Forms::Button^ buttonWhiteKnight;

private: System::Windows::Forms::Button^ buttonWhiteBishop;

private: System::Windows::Forms::Button^ buttonWhiteRook;
private: System::Windows::Forms::Button^ buttonWhiteQueen;
private: System::Windows::Forms::Label^ labelSelectedPiece;
private: System::Windows::Forms::Button^ buttonSwitchTurn;
private: System::Windows::Forms::Label^ label1;
private: System::Windows::Forms::TextBox^ textBox1;
private: System::Windows::Forms::Button^ buttonClearSquare;

private: System::Windows::Forms::Button^ buttonClearBoard;
private: System::Windows::Forms::Panel^ panel1;
private: System::Windows::Forms::Panel^ panel3;
private: System::Windows::Forms::Panel^ panel2;
private: System::Windows::Forms::OpenFileDialog^ openFileDialog;
private: System::Windows::Forms::SaveFileDialog^ saveFileDialog;
private: System::Windows::Forms::ToolStripMenuItem^ mnStop;
private: System::Windows::Forms::Label^ labelBlackTime;
private: System::Windows::Forms::Label^ labelWhiteTime;
private: System::Windows::Forms::Label^ labelWhitePieces;
private: System::Windows::Forms::Label^ labelBlackPieces;
private: System::Windows::Forms::ToolStripMenuItem^ mnGotoStart;
private: System::Windows::Forms::ToolStripMenuItem^ mnGotoEnd;


















	protected:

	protected:


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->menuStripMain = (gcnew System::Windows::Forms::MenuStrip());
			this->gameToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnNew = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnOpen = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnSave = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnSaveAs = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem7 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mnExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnGotoStart = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnStepBack = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnStepForward = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnGotoEnd = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem13 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->mnThink = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnSetup = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnReverse = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem5 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mnTwoPlayers = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem15 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripMenuItem6 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem8 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStripStop = (gcnew System::Windows::Forms::MenuStrip());
			this->mnStop = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->panelNormal = (gcnew System::Windows::Forms::Panel());
			this->richTextMovesHistory = (gcnew System::Windows::Forms::RichTextBox());
			this->panelNormalTop = (gcnew System::Windows::Forms::Panel());
			this->panelDiffPieces = (gcnew System::Windows::Forms::Panel());
			this->labelBlackPieces = (gcnew System::Windows::Forms::Label());
			this->labelWhitePieces = (gcnew System::Windows::Forms::Label());
			this->panelTimes = (gcnew System::Windows::Forms::Panel());
			this->labelBlackTime = (gcnew System::Windows::Forms::Label());
			this->labelWhiteTime = (gcnew System::Windows::Forms::Label());
			this->panelSetup = (gcnew System::Windows::Forms::Panel());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->buttonClearBoard = (gcnew System::Windows::Forms::Button());
			this->buttonClearSquare = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->buttonSwitchTurn = (gcnew System::Windows::Forms::Button());
			this->buttonBlackPawn = (gcnew System::Windows::Forms::Button());
			this->buttonBlackKnight = (gcnew System::Windows::Forms::Button());
			this->buttonBlackBishop = (gcnew System::Windows::Forms::Button());
			this->buttonBlackRook = (gcnew System::Windows::Forms::Button());
			this->buttonBlackQueen = (gcnew System::Windows::Forms::Button());
			this->buttonBlackKing = (gcnew System::Windows::Forms::Button());
			this->buttonWhitePawn = (gcnew System::Windows::Forms::Button());
			this->buttonWhiteKnight = (gcnew System::Windows::Forms::Button());
			this->buttonWhiteBishop = (gcnew System::Windows::Forms::Button());
			this->buttonWhiteRook = (gcnew System::Windows::Forms::Button());
			this->buttonWhiteQueen = (gcnew System::Windows::Forms::Button());
			this->buttonWhiteKing = (gcnew System::Windows::Forms::Button());
			this->panelSelectedPiece = (gcnew System::Windows::Forms::Panel());
			this->labelSelectedPiece = (gcnew System::Windows::Forms::Label());
			this->buttonSetupDone = (gcnew System::Windows::Forms::Button());
			this->buttonCancelSetup = (gcnew System::Windows::Forms::Button());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->menuStripMain->SuspendLayout();
			this->menuStripStop->SuspendLayout();
			this->panelNormal->SuspendLayout();
			this->panelNormalTop->SuspendLayout();
			this->panelDiffPieces->SuspendLayout();
			this->panelTimes->SuspendLayout();
			this->panelSetup->SuspendLayout();
			this->panelSelectedPiece->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStripMain
			// 
			this->menuStripMain->GripMargin = System::Windows::Forms::Padding(2, 2, 0, 2);
			this->menuStripMain->ImageScalingSize = System::Drawing::Size(24, 24);
			this->menuStripMain->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->gameToolStripMenuItem,
					this->toolStripMenuItem3, this->toolStripMenuItem4, this->toolStripMenuItem5, this->toolStripMenuItem6
			});
			this->menuStripMain->Location = System::Drawing::Point(0, 0);
			this->menuStripMain->Name = L"menuStripMain";
			this->menuStripMain->Size = System::Drawing::Size(1478, 33);
			this->menuStripMain->TabIndex = 0;
			this->menuStripMain->Text = L"menuStrip2";
			// 
			// gameToolStripMenuItem
			// 
			this->gameToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->mnNew,
					this->mnOpen, this->mnSave, this->mnSaveAs, this->toolStripMenuItem7, this->mnExit
			});
			this->gameToolStripMenuItem->Name = L"gameToolStripMenuItem";
			this->gameToolStripMenuItem->Size = System::Drawing::Size(74, 29);
			this->gameToolStripMenuItem->Text = L"&Game";
			// 
			// mnNew
			// 
			this->mnNew->Name = L"mnNew";
			this->mnNew->Size = System::Drawing::Size(185, 34);
			this->mnNew->Text = L"&New";
			this->mnNew->Click += gcnew System::EventHandler(this, &MyForm::mnNew_Click);
			// 
			// mnOpen
			// 
			this->mnOpen->Name = L"mnOpen";
			this->mnOpen->Size = System::Drawing::Size(185, 34);
			this->mnOpen->Text = L"&Open...";
			this->mnOpen->Click += gcnew System::EventHandler(this, &MyForm::mnOpen_Click);
			// 
			// mnSave
			// 
			this->mnSave->Enabled = false;
			this->mnSave->Name = L"mnSave";
			this->mnSave->Size = System::Drawing::Size(185, 34);
			this->mnSave->Text = L"&Save";
			this->mnSave->Click += gcnew System::EventHandler(this, &MyForm::mnSave_Click);
			// 
			// mnSaveAs
			// 
			this->mnSaveAs->Name = L"mnSaveAs";
			this->mnSaveAs->Size = System::Drawing::Size(185, 34);
			this->mnSaveAs->Text = L"&Save as...";
			this->mnSaveAs->Click += gcnew System::EventHandler(this, &MyForm::mnSaveAs_Click);
			// 
			// toolStripMenuItem7
			// 
			this->toolStripMenuItem7->Name = L"toolStripMenuItem7";
			this->toolStripMenuItem7->Size = System::Drawing::Size(182, 6);
			// 
			// mnExit
			// 
			this->mnExit->Name = L"mnExit";
			this->mnExit->Size = System::Drawing::Size(185, 34);
			this->mnExit->Text = L"E&xit";
			this->mnExit->Click += gcnew System::EventHandler(this, &MyForm::mnExit_Click);
			// 
			// toolStripMenuItem3
			// 
			this->toolStripMenuItem3->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->mnGotoStart,
					this->mnStepBack, this->mnStepForward, this->mnGotoEnd, this->toolStripMenuItem13, this->mnThink
			});
			this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
			this->toolStripMenuItem3->Size = System::Drawing::Size(60, 29);
			this->toolStripMenuItem3->Text = L"&Play";
			// 
			// mnGotoStart
			// 
			this->mnGotoStart->Name = L"mnGotoStart";
			this->mnGotoStart->Size = System::Drawing::Size(307, 34);
			this->mnGotoStart->Text = L"Go to Start";
			// 
			// mnStepBack
			// 
			this->mnStepBack->Name = L"mnStepBack";
			this->mnStepBack->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::Left));
			this->mnStepBack->Size = System::Drawing::Size(307, 34);
			this->mnStepBack->Text = L"Step &Back";
			this->mnStepBack->Click += gcnew System::EventHandler(this, &MyForm::mnStepBack_Click);
			// 
			// mnStepForward
			// 
			this->mnStepForward->Name = L"mnStepForward";
			this->mnStepForward->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Alt | System::Windows::Forms::Keys::Right));
			this->mnStepForward->Size = System::Drawing::Size(307, 34);
			this->mnStepForward->Text = L"Step &Forward";
			this->mnStepForward->Click += gcnew System::EventHandler(this, &MyForm::mnStepForward_Click);
			// 
			// mnGotoEnd
			// 
			this->mnGotoEnd->Name = L"mnGotoEnd";
			this->mnGotoEnd->Size = System::Drawing::Size(307, 34);
			this->mnGotoEnd->Text = L"Go to End";
			// 
			// toolStripMenuItem13
			// 
			this->toolStripMenuItem13->Name = L"toolStripMenuItem13";
			this->toolStripMenuItem13->Size = System::Drawing::Size(304, 6);
			// 
			// mnThink
			// 
			this->mnThink->Name = L"mnThink";
			this->mnThink->Size = System::Drawing::Size(307, 34);
			this->mnThink->Text = L"&Think";
			this->mnThink->Click += gcnew System::EventHandler(this, &MyForm::mnThink_Click);
			// 
			// toolStripMenuItem4
			// 
			this->toolStripMenuItem4->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->mnSetup,
					this->mnReverse
			});
			this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
			this->toolStripMenuItem4->Size = System::Drawing::Size(75, 29);
			this->toolStripMenuItem4->Text = L"&Board";
			// 
			// mnSetup
			// 
			this->mnSetup->Name = L"mnSetup";
			this->mnSetup->Size = System::Drawing::Size(174, 34);
			this->mnSetup->Text = L"&Setup...";
			this->mnSetup->Click += gcnew System::EventHandler(this, &MyForm::mnSetup_Click);
			// 
			// mnReverse
			// 
			this->mnReverse->Name = L"mnReverse";
			this->mnReverse->Size = System::Drawing::Size(174, 34);
			this->mnReverse->Tag = L"";
			this->mnReverse->Text = L"&Reverse";
			this->mnReverse->Click += gcnew System::EventHandler(this, &MyForm::mnReverse_Click);
			// 
			// toolStripMenuItem5
			// 
			this->toolStripMenuItem5->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->mnTwoPlayers,
					this->toolStripMenuItem15
			});
			this->toolStripMenuItem5->Name = L"toolStripMenuItem5";
			this->toolStripMenuItem5->Size = System::Drawing::Size(67, 29);
			this->toolStripMenuItem5->Text = L"&Level";
			// 
			// mnTwoPlayers
			// 
			this->mnTwoPlayers->Name = L"mnTwoPlayers";
			this->mnTwoPlayers->Size = System::Drawing::Size(207, 34);
			this->mnTwoPlayers->Tag = L"0";
			this->mnTwoPlayers->Text = L"&Two players";
			this->mnTwoPlayers->Click += gcnew System::EventHandler(this, &MyForm::mnTwoPlayers_Click);
			// 
			// toolStripMenuItem15
			// 
			this->toolStripMenuItem15->Name = L"toolStripMenuItem15";
			this->toolStripMenuItem15->Size = System::Drawing::Size(204, 6);
			// 
			// toolStripMenuItem6
			// 
			this->toolStripMenuItem6->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->toolStripMenuItem8 });
			this->toolStripMenuItem6->Name = L"toolStripMenuItem6";
			this->toolStripMenuItem6->Size = System::Drawing::Size(78, 29);
			this->toolStripMenuItem6->Text = L"&About";
			// 
			// toolStripMenuItem8
			// 
			this->toolStripMenuItem8->Name = L"toolStripMenuItem8";
			this->toolStripMenuItem8->Size = System::Drawing::Size(289, 34);
			this->toolStripMenuItem8->Text = L"About &Sharokin Chess";
			// 
			// menuStripStop
			// 
			this->menuStripStop->GripMargin = System::Windows::Forms::Padding(2, 2, 0, 2);
			this->menuStripStop->ImageScalingSize = System::Drawing::Size(24, 24);
			this->menuStripStop->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->mnStop });
			this->menuStripStop->Location = System::Drawing::Point(0, 0);
			this->menuStripStop->Name = L"menuStripStop";
			this->menuStripStop->Size = System::Drawing::Size(200, 24);
			this->menuStripStop->TabIndex = 0;
			// 
			// mnStop
			// 
			this->mnStop->Name = L"mnStop";
			this->mnStop->Size = System::Drawing::Size(65, 20);
			this->mnStop->Text = L"&Stop";
			this->mnStop->Click += gcnew System::EventHandler(this, &MyForm::mnStop_Click);
			// 
			// panelNormal
			// 
			this->panelNormal->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->panelNormal->Controls->Add(this->richTextMovesHistory);
			this->panelNormal->Controls->Add(this->panelNormalTop);
			this->panelNormal->Dock = System::Windows::Forms::DockStyle::Right;
			this->panelNormal->Location = System::Drawing::Point(1158, 33);
			this->panelNormal->Margin = System::Windows::Forms::Padding(0);
			this->panelNormal->Name = L"panelNormal";
			this->panelNormal->Size = System::Drawing::Size(320, 655);
			this->panelNormal->TabIndex = 1;
			// 
			// richTextMovesHistory
			// 
			this->richTextMovesHistory->Dock = System::Windows::Forms::DockStyle::Fill;
			this->richTextMovesHistory->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->richTextMovesHistory->Location = System::Drawing::Point(0, 89);
			this->richTextMovesHistory->Margin = System::Windows::Forms::Padding(0);
			this->richTextMovesHistory->Name = L"richTextMovesHistory";
			this->richTextMovesHistory->ReadOnly = true;
			this->richTextMovesHistory->Size = System::Drawing::Size(320, 566);
			this->richTextMovesHistory->TabIndex = 2;
			this->richTextMovesHistory->Text = L"";
			this->richTextMovesHistory->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::richTextMovesHistory_MouseClick);
			this->richTextMovesHistory->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::richTextMovesHistory_MouseClick);
			// 
			// panelNormalTop
			// 
			this->panelNormalTop->BackColor = System::Drawing::SystemColors::AppWorkspace;
			this->panelNormalTop->Controls->Add(this->panelDiffPieces);
			this->panelNormalTop->Controls->Add(this->panelTimes);
			this->panelNormalTop->Dock = System::Windows::Forms::DockStyle::Top;
			this->panelNormalTop->Location = System::Drawing::Point(0, 0);
			this->panelNormalTop->Margin = System::Windows::Forms::Padding(0);
			this->panelNormalTop->Name = L"panelNormalTop";
			this->panelNormalTop->Size = System::Drawing::Size(320, 89);
			this->panelNormalTop->TabIndex = 1;
			// 
			// panelDiffPieces
			// 
			this->panelDiffPieces->BackColor = System::Drawing::SystemColors::ControlLight;
			this->panelDiffPieces->Controls->Add(this->labelBlackPieces);
			this->panelDiffPieces->Controls->Add(this->labelWhitePieces);
			this->panelDiffPieces->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panelDiffPieces->Location = System::Drawing::Point(133, 0);
			this->panelDiffPieces->Margin = System::Windows::Forms::Padding(0);
			this->panelDiffPieces->Name = L"panelDiffPieces";
			this->panelDiffPieces->Size = System::Drawing::Size(187, 89);
			this->panelDiffPieces->TabIndex = 1;
			// 
			// labelBlackPieces
			// 
			this->labelBlackPieces->AutoSize = true;
			this->labelBlackPieces->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->labelBlackPieces->Location = System::Drawing::Point(0, 50);
			this->labelBlackPieces->Name = L"labelBlackPieces";
			this->labelBlackPieces->Size = System::Drawing::Size(55, 32);
			this->labelBlackPieces->TabIndex = 5;
			this->labelBlackPieces->Text = L"K(2)";
			// 
			// labelWhitePieces
			// 
			this->labelWhitePieces->AutoSize = true;
			this->labelWhitePieces->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->labelWhitePieces->Location = System::Drawing::Point(0, 0);
			this->labelWhitePieces->Name = L"labelWhitePieces";
			this->labelWhitePieces->Size = System::Drawing::Size(28, 32);
			this->labelWhitePieces->TabIndex = 4;
			this->labelWhitePieces->Text = L"K";
			// 
			// panelTimes
			// 
			this->panelTimes->BackColor = System::Drawing::Color::Gray;
			this->panelTimes->Controls->Add(this->labelBlackTime);
			this->panelTimes->Controls->Add(this->labelWhiteTime);
			this->panelTimes->Dock = System::Windows::Forms::DockStyle::Left;
			this->panelTimes->Location = System::Drawing::Point(0, 0);
			this->panelTimes->Margin = System::Windows::Forms::Padding(0);
			this->panelTimes->Name = L"panelTimes";
			this->panelTimes->Size = System::Drawing::Size(133, 89);
			this->panelTimes->TabIndex = 0;
			// 
			// labelBlackTime
			// 
			this->labelBlackTime->AutoSize = true;
			this->labelBlackTime->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->labelBlackTime->Location = System::Drawing::Point(0, 50);
			this->labelBlackTime->Name = L"labelBlackTime";
			this->labelBlackTime->Size = System::Drawing::Size(143, 37);
			this->labelBlackTime->TabIndex = 3;
			this->labelBlackTime->Text = L"00:00:00";
			// 
			// labelWhiteTime
			// 
			this->labelWhiteTime->AutoSize = true;
			this->labelWhiteTime->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->labelWhiteTime->ForeColor = System::Drawing::Color::White;
			this->labelWhiteTime->Location = System::Drawing::Point(0, 0);
			this->labelWhiteTime->Name = L"labelWhiteTime";
			this->labelWhiteTime->Size = System::Drawing::Size(143, 37);
			this->labelWhiteTime->TabIndex = 2;
			this->labelWhiteTime->Text = L"00:00:00";
			this->labelWhiteTime->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// panelSetup
			// 
			this->panelSetup->BackColor = System::Drawing::SystemColors::Control;
			this->panelSetup->Controls->Add(this->panel3);
			this->panelSetup->Controls->Add(this->panel2);
			this->panelSetup->Controls->Add(this->panel1);
			this->panelSetup->Controls->Add(this->buttonClearBoard);
			this->panelSetup->Controls->Add(this->buttonClearSquare);
			this->panelSetup->Controls->Add(this->label1);
			this->panelSetup->Controls->Add(this->textBox1);
			this->panelSetup->Controls->Add(this->buttonSwitchTurn);
			this->panelSetup->Controls->Add(this->buttonBlackPawn);
			this->panelSetup->Controls->Add(this->buttonBlackKnight);
			this->panelSetup->Controls->Add(this->buttonBlackBishop);
			this->panelSetup->Controls->Add(this->buttonBlackRook);
			this->panelSetup->Controls->Add(this->buttonBlackQueen);
			this->panelSetup->Controls->Add(this->buttonBlackKing);
			this->panelSetup->Controls->Add(this->buttonWhitePawn);
			this->panelSetup->Controls->Add(this->buttonWhiteKnight);
			this->panelSetup->Controls->Add(this->buttonWhiteBishop);
			this->panelSetup->Controls->Add(this->buttonWhiteRook);
			this->panelSetup->Controls->Add(this->buttonWhiteQueen);
			this->panelSetup->Controls->Add(this->buttonWhiteKing);
			this->panelSetup->Controls->Add(this->panelSelectedPiece);
			this->panelSetup->Controls->Add(this->buttonSetupDone);
			this->panelSetup->Controls->Add(this->buttonCancelSetup);
			this->panelSetup->Dock = System::Windows::Forms::DockStyle::Right;
			this->panelSetup->Location = System::Drawing::Point(838, 33);
			this->panelSetup->Margin = System::Windows::Forms::Padding(0);
			this->panelSetup->Name = L"panelSetup";
			this->panelSetup->Size = System::Drawing::Size(320, 655);
			this->panelSetup->TabIndex = 2;
			// 
			// panel3
			// 
			this->panel3->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel3->Location = System::Drawing::Point(15, 428);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(290, 1);
			this->panel3->TabIndex = 34;
			// 
			// panel2
			// 
			this->panel2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel2->Location = System::Drawing::Point(15, 378);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(290, 1);
			this->panel2->TabIndex = 33;
			// 
			// panel1
			// 
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel1->Location = System::Drawing::Point(15, 56);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(290, 1);
			this->panel1->TabIndex = 32;
			// 
			// buttonClearBoard
			// 
			this->buttonClearBoard->Location = System::Drawing::Point(15, 15);
			this->buttonClearBoard->Name = L"buttonClearBoard";
			this->buttonClearBoard->Size = System::Drawing::Size(140, 32);
			this->buttonClearBoard->TabIndex = 31;
			this->buttonClearBoard->Text = L"&Clear Board";
			this->buttonClearBoard->UseVisualStyleBackColor = true;
			this->buttonClearBoard->Click += gcnew System::EventHandler(this, &MyForm::buttonClearBoard_Click);
			// 
			// buttonClearSquare
			// 
			this->buttonClearSquare->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 8));
			this->buttonClearSquare->Location = System::Drawing::Point(166, 15);
			this->buttonClearSquare->Name = L"buttonClearSquare";
			this->buttonClearSquare->Size = System::Drawing::Size(140, 32);
			this->buttonClearSquare->TabIndex = 30;
			this->buttonClearSquare->Text = L"Clear &Square";
			this->buttonClearSquare->UseVisualStyleBackColor = true;
			this->buttonClearSquare->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupPiece_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(18, 392);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(105, 25);
			this->label1->TabIndex = 29;
			this->label1->Text = L"&Enpassant";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(106, 389);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(32, 30);
			this->textBox1->TabIndex = 28;
			// 
			// buttonSwitchTurn
			// 
			this->buttonSwitchTurn->Location = System::Drawing::Point(156, 387);
			this->buttonSwitchTurn->Name = L"buttonSwitchTurn";
			this->buttonSwitchTurn->Size = System::Drawing::Size(140, 32);
			this->buttonSwitchTurn->TabIndex = 27;
			this->buttonSwitchTurn->Text = L"&Turn: White";
			this->buttonSwitchTurn->UseVisualStyleBackColor = true;
			this->buttonSwitchTurn->Click += gcnew System::EventHandler(this, &MyForm::buttonSwitchTurn_Click);
			// 
			// buttonBlackPawn
			// 
			this->buttonBlackPawn->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 24));
			this->buttonBlackPawn->Location = System::Drawing::Point(71, 317);
			this->buttonBlackPawn->Name = L"buttonBlackPawn";
			this->buttonBlackPawn->Size = System::Drawing::Size(50, 50);
			this->buttonBlackPawn->TabIndex = 26;
			this->buttonBlackPawn->Text = L"P";
			this->buttonBlackPawn->UseVisualStyleBackColor = true;
			this->buttonBlackPawn->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupPiece_Click);
			// 
			// buttonBlackKnight
			// 
			this->buttonBlackKnight->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 24));
			this->buttonBlackKnight->Location = System::Drawing::Point(71, 267);
			this->buttonBlackKnight->Name = L"buttonBlackKnight";
			this->buttonBlackKnight->Size = System::Drawing::Size(50, 50);
			this->buttonBlackKnight->TabIndex = 25;
			this->buttonBlackKnight->Text = L"N";
			this->buttonBlackKnight->UseVisualStyleBackColor = true;
			this->buttonBlackKnight->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupPiece_Click);
			// 
			// buttonBlackBishop
			// 
			this->buttonBlackBishop->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 24));
			this->buttonBlackBishop->Location = System::Drawing::Point(71, 217);
			this->buttonBlackBishop->Name = L"buttonBlackBishop";
			this->buttonBlackBishop->Size = System::Drawing::Size(50, 50);
			this->buttonBlackBishop->TabIndex = 24;
			this->buttonBlackBishop->Text = L"B";
			this->buttonBlackBishop->UseVisualStyleBackColor = true;
			this->buttonBlackBishop->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupPiece_Click);
			// 
			// buttonBlackRook
			// 
			this->buttonBlackRook->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 24));
			this->buttonBlackRook->Location = System::Drawing::Point(71, 167);
			this->buttonBlackRook->Name = L"buttonBlackRook";
			this->buttonBlackRook->Size = System::Drawing::Size(50, 50);
			this->buttonBlackRook->TabIndex = 23;
			this->buttonBlackRook->Text = L"R";
			this->buttonBlackRook->UseVisualStyleBackColor = true;
			this->buttonBlackRook->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupPiece_Click);
			// 
			// buttonBlackQueen
			// 
			this->buttonBlackQueen->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 24));
			this->buttonBlackQueen->Location = System::Drawing::Point(71, 117);
			this->buttonBlackQueen->Name = L"buttonBlackQueen";
			this->buttonBlackQueen->Size = System::Drawing::Size(50, 50);
			this->buttonBlackQueen->TabIndex = 22;
			this->buttonBlackQueen->Text = L"Q";
			this->buttonBlackQueen->UseVisualStyleBackColor = true;
			this->buttonBlackQueen->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupPiece_Click);
			// 
			// buttonBlackKing
			// 
			this->buttonBlackKing->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 24));
			this->buttonBlackKing->Location = System::Drawing::Point(71, 67);
			this->buttonBlackKing->Name = L"buttonBlackKing";
			this->buttonBlackKing->Size = System::Drawing::Size(50, 50);
			this->buttonBlackKing->TabIndex = 21;
			this->buttonBlackKing->Text = L"K";
			this->buttonBlackKing->UseVisualStyleBackColor = true;
			this->buttonBlackKing->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupPiece_Click);
			// 
			// buttonWhitePawn
			// 
			this->buttonWhitePawn->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 24));
			this->buttonWhitePawn->Location = System::Drawing::Point(15, 317);
			this->buttonWhitePawn->Name = L"buttonWhitePawn";
			this->buttonWhitePawn->Size = System::Drawing::Size(50, 50);
			this->buttonWhitePawn->TabIndex = 20;
			this->buttonWhitePawn->Text = L"P";
			this->buttonWhitePawn->UseVisualStyleBackColor = true;
			this->buttonWhitePawn->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupPiece_Click);
			// 
			// buttonWhiteKnight
			// 
			this->buttonWhiteKnight->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 24));
			this->buttonWhiteKnight->Location = System::Drawing::Point(15, 267);
			this->buttonWhiteKnight->Name = L"buttonWhiteKnight";
			this->buttonWhiteKnight->Size = System::Drawing::Size(50, 50);
			this->buttonWhiteKnight->TabIndex = 19;
			this->buttonWhiteKnight->Text = L"N";
			this->buttonWhiteKnight->UseVisualStyleBackColor = true;
			this->buttonWhiteKnight->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupPiece_Click);
			// 
			// buttonWhiteBishop
			// 
			this->buttonWhiteBishop->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 24));
			this->buttonWhiteBishop->Location = System::Drawing::Point(15, 217);
			this->buttonWhiteBishop->Name = L"buttonWhiteBishop";
			this->buttonWhiteBishop->Size = System::Drawing::Size(50, 50);
			this->buttonWhiteBishop->TabIndex = 18;
			this->buttonWhiteBishop->Text = L"B";
			this->buttonWhiteBishop->UseVisualStyleBackColor = true;
			this->buttonWhiteBishop->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupPiece_Click);
			// 
			// buttonWhiteRook
			// 
			this->buttonWhiteRook->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 24));
			this->buttonWhiteRook->Location = System::Drawing::Point(15, 167);
			this->buttonWhiteRook->Name = L"buttonWhiteRook";
			this->buttonWhiteRook->Size = System::Drawing::Size(50, 50);
			this->buttonWhiteRook->TabIndex = 17;
			this->buttonWhiteRook->Text = L"R";
			this->buttonWhiteRook->UseVisualStyleBackColor = true;
			this->buttonWhiteRook->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupPiece_Click);
			// 
			// buttonWhiteQueen
			// 
			this->buttonWhiteQueen->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 24));
			this->buttonWhiteQueen->Location = System::Drawing::Point(15, 117);
			this->buttonWhiteQueen->Name = L"buttonWhiteQueen";
			this->buttonWhiteQueen->Size = System::Drawing::Size(50, 50);
			this->buttonWhiteQueen->TabIndex = 16;
			this->buttonWhiteQueen->Text = L"Q";
			this->buttonWhiteQueen->UseVisualStyleBackColor = true;
			this->buttonWhiteQueen->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupPiece_Click);
			// 
			// buttonWhiteKing
			// 
			this->buttonWhiteKing->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 24));
			this->buttonWhiteKing->Location = System::Drawing::Point(15, 67);
			this->buttonWhiteKing->Name = L"buttonWhiteKing";
			this->buttonWhiteKing->Size = System::Drawing::Size(50, 50);
			this->buttonWhiteKing->TabIndex = 15;
			this->buttonWhiteKing->Text = L"K";
			this->buttonWhiteKing->UseVisualStyleBackColor = true;
			this->buttonWhiteKing->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupPiece_Click);
			// 
			// panelSelectedPiece
			// 
			this->panelSelectedPiece->BackColor = System::Drawing::Color::Silver;
			this->panelSelectedPiece->Controls->Add(this->labelSelectedPiece);
			this->panelSelectedPiece->Location = System::Drawing::Point(203, 70);
			this->panelSelectedPiece->Margin = System::Windows::Forms::Padding(0);
			this->panelSelectedPiece->Name = L"panelSelectedPiece";
			this->panelSelectedPiece->Size = System::Drawing::Size(100, 100);
			this->panelSelectedPiece->TabIndex = 14;
			// 
			// labelSelectedPiece
			// 
			this->labelSelectedPiece->AutoSize = true;
			this->labelSelectedPiece->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 50, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->labelSelectedPiece->Location = System::Drawing::Point(3, -1);
			this->labelSelectedPiece->Name = L"labelSelectedPiece";
			this->labelSelectedPiece->Size = System::Drawing::Size(113, 133);
			this->labelSelectedPiece->TabIndex = 0;
			this->labelSelectedPiece->Text = L"P";
			// 
			// buttonSetupDone
			// 
			this->buttonSetupDone->Location = System::Drawing::Point(15, 497);
			this->buttonSetupDone->Name = L"buttonSetupDone";
			this->buttonSetupDone->Size = System::Drawing::Size(140, 32);
			this->buttonSetupDone->TabIndex = 1;
			this->buttonSetupDone->Text = L"&Done";
			this->buttonSetupDone->UseVisualStyleBackColor = true;
			this->buttonSetupDone->Click += gcnew System::EventHandler(this, &MyForm::buttonSetupDone_Click);
			// 
			// buttonCancelSetup
			// 
			this->buttonCancelSetup->Location = System::Drawing::Point(166, 497);
			this->buttonCancelSetup->Name = L"buttonCancelSetup";
			this->buttonCancelSetup->Size = System::Drawing::Size(140, 32);
			this->buttonCancelSetup->TabIndex = 0;
			this->buttonCancelSetup->Text = L"&Cancel";
			this->buttonCancelSetup->UseVisualStyleBackColor = true;
			this->buttonCancelSetup->Click += gcnew System::EventHandler(this, &MyForm::buttonCancelSetup_Click);
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog";
			// 
			// MyForm
			// 
			this->BackColor = System::Drawing::Color::Silver;
			this->ClientSize = System::Drawing::Size(1478, 688);
			this->Controls->Add(this->panelSetup);
			this->Controls->Add(this->panelNormal);
			this->Controls->Add(this->menuStripMain);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->MainMenuStrip = this->menuStripMain;
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Sharokin Chess";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
			this->menuStripMain->ResumeLayout(false);
			this->menuStripMain->PerformLayout();
			this->menuStripStop->ResumeLayout(false);
			this->menuStripStop->PerformLayout();
			this->panelNormal->ResumeLayout(false);
			this->panelNormalTop->ResumeLayout(false);
			this->panelDiffPieces->ResumeLayout(false);
			this->panelDiffPieces->PerformLayout();
			this->panelTimes->ResumeLayout(false);
			this->panelTimes->PerformLayout();
			this->panelSetup->ResumeLayout(false);
			this->panelSetup->PerformLayout();
			this->panelSelectedPiece->ResumeLayout(false);
			this->panelSelectedPiece->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void menuStrip1_ItemClicked(System::Object^ sender, System::Windows::Forms::ToolStripItemClickedEventArgs^ e) {
	}
	private: System::Void levelToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	}
private: System::Void exitToolStripMenuItem1_Click(System::Object^ sender, System::EventArgs^ e) {
	Close();
}
	// User 
	private:
		System::String^ chessSymbolsFont;
		array<System::Drawing::Color>^ cellColors;
		array<System::Drawing::Color>^ moveColors;
		System::Drawing::Color clickColor;
		array<System::String^, 2>^ pieces = gcnew array<System::String^, 2>(2, 7);
		array<System::Windows::Forms::Panel^, 2>^ PanelsArray;
		array<System::Windows::Forms::Label^, 2>^ LabelsArray;

		array<System::Windows::Forms::Label^, 2>^ labelRankNumbers;
		array<System::Windows::Forms::Label^, 2>^ labelFileLetters;

		int NumRows;
		int NumCols;
		int FromCol;
		int FromRow;

		System::String^ savedFileName;
		array<System::Int32>^ ArrangedPos;// = gcnew array<System::Int32>(64);

		///---------- Timers Declarations ------------------------
		System::Windows::Forms::Timer^ whiteTimer;
		System::Windows::Forms::Timer^ blackTimer;
		int whiteSeconds = 0;
		int blackSeconds = 0;
		System::Void WhiteTimer_Tick(System::Object^ sender, System::EventArgs^ e);
		System::Void BlackTimer_Tick(System::Object^ sender, System::EventArgs^ e);
		///-------------------------------------------------------
		SharokinChess::PromotePawnForm^ promotePawnForm;

		System::Void InitializeBoard();
		System::Void UpdateBoard(int flag);
		System::UInt32 BoardRow(int row) { return (safe_cast<int>(mnReverse->Tag) == 1) ? row : 7 - row; }
		System::UInt32 BoardCol(int col) { return (safe_cast<int>(mnReverse->Tag) == 0) ? col : 7 - col; }
		System::Void ShowPieceOnSquare(int col, int row, int piece, int side);
		System::Void UpdateTimes();
		System::Boolean CloseGame();
		System::Boolean SaveGame(System::Boolean);
		System::Boolean ShowSaveDialog();
		System::Void OnPanel_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void OnLabel_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void OnSquare_Click(int col, int row);
		System::Void mnSetup_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void buttonCancelSetup_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void mnThink_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void mnTwoPlayers_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void buttonSetupPiece_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void buttonSwitchTurn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void mnReverse_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void buttonSetupDone_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void buttonClearBoard_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void OnAppIdle(System::Object^ sender, System::EventArgs^ e);
		System::Void mnExit_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void mnOpen_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void mnSave_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void mnSaveAs_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void mnNew_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void mnStop_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void SharokinChess::MyForm::UpdateCapturedPiece();

		System::Boolean isClosingAlready = false;
		System::Void MyForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
			if (isClosingAlready)
				return;
			if (true == CloseGame()) {
				isClosingAlready = true;
				Close();
			}
			else {
				e->Cancel = true;
			}
		}
		System::Void MakeMove();
		System::Void mnStepBack_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void mnStepForward_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void HighlightMoveString();
		System::Void richTextMovesHistory_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
};
}
