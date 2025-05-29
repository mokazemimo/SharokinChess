#pragma once

namespace SharokinChess {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for PromotePawnForm
	/// </summary>
	public ref class PromotePawnForm : public System::Windows::Forms::Form
	{
	public:
		PromotePawnForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->InitializeForm();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PromotePawnForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ buttonOk;
	private: System::Windows::Forms::Panel^ panelPromoQueen;
	protected:

	private: System::Windows::Forms::Button^ buttonPromoQueen;
	private: System::Windows::Forms::Panel^ panelPromoRook;

	private: System::Windows::Forms::Button^ buttonPromoRook;
	private: System::Windows::Forms::Panel^ panelPromoBishop;


	private: System::Windows::Forms::Button^ buttonPromoBishop;
	private: System::Windows::Forms::Panel^ panelPromoKnight;


	private: System::Windows::Forms::Button^ buttonPromoKnight;


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
			this->buttonOk = (gcnew System::Windows::Forms::Button());
			this->panelPromoQueen = (gcnew System::Windows::Forms::Panel());
			this->buttonPromoQueen = (gcnew System::Windows::Forms::Button());
			this->panelPromoRook = (gcnew System::Windows::Forms::Panel());
			this->buttonPromoRook = (gcnew System::Windows::Forms::Button());
			this->panelPromoBishop = (gcnew System::Windows::Forms::Panel());
			this->buttonPromoBishop = (gcnew System::Windows::Forms::Button());
			this->panelPromoKnight = (gcnew System::Windows::Forms::Panel());
			this->buttonPromoKnight = (gcnew System::Windows::Forms::Button());
			this->panelPromoQueen->SuspendLayout();
			this->panelPromoRook->SuspendLayout();
			this->panelPromoBishop->SuspendLayout();
			this->panelPromoKnight->SuspendLayout();
			this->SuspendLayout();
			// 
			// buttonOk
			// 
			this->buttonOk->Location = System::Drawing::Point(295, 231);
			this->buttonOk->Name = L"buttonOk";
			this->buttonOk->Size = System::Drawing::Size(155, 41);
			this->buttonOk->TabIndex = 4;
			this->buttonOk->Text = L"&Ok";
			this->buttonOk->UseVisualStyleBackColor = true;
			this->buttonOk->Click += gcnew System::EventHandler(this, &PromotePawnForm::buttonOk_Click);
			// 
			// panelPromoQueen
			// 
			this->panelPromoQueen->BackColor = System::Drawing::Color::Navy;
			this->panelPromoQueen->Controls->Add(this->buttonPromoQueen);
			this->panelPromoQueen->Location = System::Drawing::Point(50, 50);
			this->panelPromoQueen->Name = L"panelPromoQueen";
			this->panelPromoQueen->Size = System::Drawing::Size(160, 160);
			this->panelPromoQueen->TabIndex = 5;
			// 
			// buttonPromoQueen
			// 
			this->buttonPromoQueen->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 40, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonPromoQueen->Location = System::Drawing::Point(20, 20);
			this->buttonPromoQueen->Margin = System::Windows::Forms::Padding(0);
			this->buttonPromoQueen->Name = L"buttonPromoQueen";
			this->buttonPromoQueen->Size = System::Drawing::Size(120, 120);
			this->buttonPromoQueen->TabIndex = 1;
			this->buttonPromoQueen->Text = L"Q";
			this->buttonPromoQueen->UseVisualStyleBackColor = true;
			this->buttonPromoQueen->Click += gcnew System::EventHandler(this, &PromotePawnForm::buttonPromo_Click);
			// 
			// panelPromoRook
			// 
			this->panelPromoRook->Controls->Add(this->buttonPromoRook);
			this->panelPromoRook->Location = System::Drawing::Point(216, 50);
			this->panelPromoRook->Name = L"panelPromoRook";
			this->panelPromoRook->Size = System::Drawing::Size(160, 160);
			this->panelPromoRook->TabIndex = 6;
			// 
			// buttonPromoRook
			// 
			this->buttonPromoRook->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 40, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonPromoRook->Location = System::Drawing::Point(20, 20);
			this->buttonPromoRook->Margin = System::Windows::Forms::Padding(0);
			this->buttonPromoRook->Name = L"buttonPromoRook";
			this->buttonPromoRook->Size = System::Drawing::Size(120, 120);
			this->buttonPromoRook->TabIndex = 1;
			this->buttonPromoRook->Text = L"R";
			this->buttonPromoRook->UseVisualStyleBackColor = true;
			this->buttonPromoRook->Click += gcnew System::EventHandler(this, &PromotePawnForm::buttonPromo_Click);
			// 
			// panelPromoBishop
			// 
			this->panelPromoBishop->Controls->Add(this->buttonPromoBishop);
			this->panelPromoBishop->Location = System::Drawing::Point(382, 50);
			this->panelPromoBishop->Name = L"panelPromoBishop";
			this->panelPromoBishop->Size = System::Drawing::Size(160, 160);
			this->panelPromoBishop->TabIndex = 7;
			// 
			// buttonPromoBishop
			// 
			this->buttonPromoBishop->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 40, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonPromoBishop->Location = System::Drawing::Point(20, 20);
			this->buttonPromoBishop->Margin = System::Windows::Forms::Padding(0);
			this->buttonPromoBishop->Name = L"buttonPromoBishop";
			this->buttonPromoBishop->Size = System::Drawing::Size(120, 120);
			this->buttonPromoBishop->TabIndex = 1;
			this->buttonPromoBishop->Text = L"B";
			this->buttonPromoBishop->UseVisualStyleBackColor = true;
			this->buttonPromoBishop->Click += gcnew System::EventHandler(this, &PromotePawnForm::buttonPromo_Click);
			// 
			// panelPromoKnight
			// 
			this->panelPromoKnight->Controls->Add(this->buttonPromoKnight);
			this->panelPromoKnight->Location = System::Drawing::Point(548, 50);
			this->panelPromoKnight->Name = L"panelPromoKnight";
			this->panelPromoKnight->Size = System::Drawing::Size(160, 160);
			this->panelPromoKnight->TabIndex = 8;
			// 
			// buttonPromoKnight
			// 
			this->buttonPromoKnight->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 40, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonPromoKnight->Location = System::Drawing::Point(20, 20);
			this->buttonPromoKnight->Margin = System::Windows::Forms::Padding(0);
			this->buttonPromoKnight->Name = L"buttonPromoKnight";
			this->buttonPromoKnight->Size = System::Drawing::Size(120, 120);
			this->buttonPromoKnight->TabIndex = 1;
			this->buttonPromoKnight->Text = L"N";
			this->buttonPromoKnight->UseVisualStyleBackColor = true;
			this->buttonPromoKnight->Click += gcnew System::EventHandler(this, &PromotePawnForm::buttonPromo_Click);
			// 
			// PromotePawnForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(761, 309);
			this->Controls->Add(this->panelPromoKnight);
			this->Controls->Add(this->panelPromoBishop);
			this->Controls->Add(this->panelPromoRook);
			this->Controls->Add(this->panelPromoQueen);
			this->Controls->Add(this->buttonOk);
			this->Name = L"PromotePawnForm";
			this->Text = L"PromotePawnForm";
			this->Shown += gcnew System::EventHandler(this, &PromotePawnForm::PromotePawnForm_Shown);
			this->panelPromoQueen->ResumeLayout(false);
			this->panelPromoRook->ResumeLayout(false);
			this->panelPromoBishop->ResumeLayout(false);
			this->panelPromoKnight->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
		System::Void InitializeForm();
		System::Void buttonOk_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void PromotePawnForm_Shown(System::Object^ sender, System::EventArgs^ e);
		System::Void buttonPromo_Click(System::Object^ sender, System::EventArgs^ e);
};
}
