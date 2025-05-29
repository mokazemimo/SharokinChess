#include "PromotePawnForm.h"
#include "myBoard.h"

extern CBoard& board;

System::Void SharokinChess::PromotePawnForm::InitializeForm()
{
	buttonPromoQueen->Tag = PIECE_QUEEN;
	buttonPromoRook->Tag = PIECE_ROOK;
	buttonPromoBishop->Tag = PIECE_BISHOP;
	buttonPromoKnight->Tag = PIECE_KNIGHT;
	this->Tag = 0;
}

System::Void SharokinChess::PromotePawnForm::buttonOk_Click(System::Object^ sender, System::EventArgs^ e)
{
	/*
	switch (safe_cast<int>(Tag))
	{
	case 0:
		board.setPromotion(PIECE_QUEEN);  break;
	case 1:
		board.setPromotion(PIECE_ROOK);  break;
	case 2:
		board.setPromotion(PIECE_BISHOP);  break;
	case 3:
		board.setPromotion(PIECE_KNIGHT);  break;
	}
	*/
	board.setPromotion(safe_cast<int>(Tag));
	Close();
}

System::Void SharokinChess::PromotePawnForm::PromotePawnForm_Shown(System::Object^ sender, System::EventArgs^ e)
{
	panelPromoQueen->BackColor = System::Drawing::Color::Navy;
	panelPromoRook->BackColor = this->BackColor;
	panelPromoBishop->BackColor = this->BackColor;
	panelPromoKnight->BackColor = this->BackColor;
	this->Tag = PIECE_QUEEN;

    if (board.getTurn() == SIDE_WHITE)
    {
		buttonPromoQueen->Text = L"\u2655";
		buttonPromoRook->Text = L"\u2656";
		buttonPromoBishop->Text = L"\u2657";
		buttonPromoKnight->Text = L"\u2658";
	}
    else
    {
		buttonPromoQueen->Text = L"\u265B";
		buttonPromoRook->Text = L"\u265C";
		buttonPromoBishop->Text = L"\u265D";
		buttonPromoKnight->Text = L"\u265E";
	}
}

System::Void SharokinChess::PromotePawnForm::buttonPromo_Click(System::Object^ sender, System::EventArgs^ e)
{
	System::Windows::Forms::Button^ clickedButton = safe_cast<System::Windows::Forms::Button^>(sender);
	this->Tag = safe_cast<int>(clickedButton->Tag);

	switch (safe_cast<int>(this->Tag))
	{
	case PIECE_QUEEN:
		panelPromoQueen->BackColor = System::Drawing::Color::Navy;
		panelPromoRook->BackColor = this->BackColor;
		panelPromoBishop->BackColor = this->BackColor;
		panelPromoKnight->BackColor = this->BackColor;
		break;
	case PIECE_ROOK:
		panelPromoRook->BackColor = System::Drawing::Color::Navy;
		panelPromoQueen->BackColor = this->BackColor;
		panelPromoBishop->BackColor = this->BackColor;
		panelPromoKnight->BackColor = this->BackColor;
		break;
	case PIECE_BISHOP:
		panelPromoBishop->BackColor = System::Drawing::Color::Navy;
		panelPromoQueen->BackColor = this->BackColor;
		panelPromoRook->BackColor = this->BackColor;
		panelPromoKnight->BackColor = this->BackColor;
		break;
	case PIECE_KNIGHT:
		panelPromoKnight->BackColor = System::Drawing::Color::Navy;
		panelPromoQueen->BackColor = this->BackColor;
		panelPromoRook->BackColor = this->BackColor;
		panelPromoBishop->BackColor = this->BackColor;
		break;
	default:
		break;
	}
}
