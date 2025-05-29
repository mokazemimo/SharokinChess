
#include <stdio.h>
#include "MyForm.h"
#include "PromotePawnForm.h"

#include "myBoard.h"

CBoard &board = CBoard::instance();

long bThinking;

void SharokinChess::MyForm::InitializeBoard()
{
    chessSymbolsFont = gcnew System::String("Segoe UI Symbol");
    float chessFontSize = 38.0f;
    
    // Define colors: index 0 = white square, index 1 = black square
    cellColors = gcnew array<System::Drawing::Color>(2);
    cellColors[0] = System::Drawing::Color::FromArgb(240, 240, 240); //System::Drawing::Color::Beige;
    cellColors[1] = System::Drawing::Color::FromArgb(144, 192, 150); //System::Drawing::Color::SaddleBrown;

    // Define colors: index 0 = white square, index 1 = black square
    moveColors = gcnew array<System::Drawing::Color>(2);
    moveColors[0] = System::Drawing::Color::FromArgb(255, 224, 96);    // move-from color
    moveColors[1] = System::Drawing::Color::FromArgb(248, 192, 32);    // move-to color

    clickColor = System::Drawing::Color::FromArgb(248, 208, 0);    // click color

    HWND hWnd = static_cast<HWND>(this->Handle.ToPointer());
    board.LoadData(hWnd);
    board.ReadOpenBook("sh000.obk");
    board.SetupBoard();

    NumRows = 8;
    NumCols = 8;

    System::EventHandler^ idleHandler = gcnew System::EventHandler(this, &MyForm::OnAppIdle);
    Application::Idle += idleHandler;

    ///----------------setup Timers --------------
    whiteTimer = gcnew System::Windows::Forms::Timer();
    blackTimer = gcnew System::Windows::Forms::Timer();

    whiteTimer->Interval = 1000;  // 1000 ms = 1 second
    blackTimer->Interval = 1000;

    whiteTimer->Tick += gcnew System::EventHandler(this, &MyForm::WhiteTimer_Tick);
    blackTimer->Tick += gcnew System::EventHandler(this, &MyForm::BlackTimer_Tick);
    ///------------------------------

    int width = 60, edge = 36, top = edge + 30;

    // Unicodes of chess pieces
    pieces[0, 0] = L"\u2654"; // King
    pieces[0, 1] = L"\u2655"; // Queen
    pieces[0, 3] = L"\u2656"; // Rook
    pieces[0, 4] = L"\u2657"; // Bishop
    pieces[0, 2] = L"\u2658"; // Knight
    pieces[0, 5] = L"\u2659"; // Pawn
    pieces[0, 6] = L"\u2659"; // Pawn

    // Black pieces (row 1)
    pieces[1, 0] = L"\u265A"; // King
    pieces[1, 1] = L"\u265B"; // Queen
    pieces[1, 3] = L"\u265C"; // Rook
    pieces[1, 4] = L"\u265D"; // Bishop
    pieces[1, 2] = L"\u265E"; // Knight
    pieces[1, 5] = L"\u265F"; // Pawn
    pieces[1, 6] = L"\u265F"; // Pawn

    labelWhitePieces->Font = gcnew System::Drawing::Font(chessSymbolsFont, 12);
    labelBlackPieces->Font = gcnew System::Drawing::Font(chessSymbolsFont, 12);

    buttonWhiteKing->Text = L"\u2654";
    buttonWhiteKing->Tag = 10 * SIDE_WHITE + PIECE_KING;
    buttonWhiteQueen->Text = L"\u2655";
    buttonWhiteQueen->Tag = 10 * SIDE_WHITE + PIECE_QUEEN;
    buttonWhiteRook->Text = L"\u2656";
    buttonWhiteRook->Tag = 10 * SIDE_WHITE + PIECE_ROOK;
    buttonWhiteBishop->Text = L"\u2657";
    buttonWhiteBishop->Tag = 10 * SIDE_WHITE + PIECE_BISHOP;
    buttonWhiteKnight->Text = L"\u2658";
    buttonWhiteKnight->Tag = 10 * SIDE_WHITE + PIECE_KNIGHT;
    buttonWhitePawn->Text = L"\u2659";
    buttonWhitePawn->Tag = 10 * SIDE_WHITE + PIECE_PAWN(SIDE_WHITE);

    buttonBlackKing->Text = L"\u265A";
    buttonBlackKing->Tag = 10 * SIDE_BLACK + PIECE_KING;
    buttonBlackQueen->Text = L"\u265B";
    buttonBlackQueen->Tag = 10 * SIDE_BLACK + PIECE_QUEEN;
    buttonBlackRook->Text = L"\u265C";
    buttonBlackRook->Tag = 10 * SIDE_BLACK + PIECE_ROOK;
    buttonBlackBishop->Text = L"\u265D";
    buttonBlackBishop->Tag = 10 * SIDE_BLACK + PIECE_BISHOP;
    buttonBlackKnight->Text = L"\u265E";
    buttonBlackKnight->Tag = 10 * SIDE_BLACK + PIECE_KNIGHT;
    buttonBlackPawn->Text = L"\u265F";
    buttonBlackPawn->Tag = 10 * SIDE_BLACK + PIECE_PAWN(SIDE_WHITE);
    buttonClearSquare->Tag = -1;

    savedFileName = gcnew System::String("");
    ArrangedPos = gcnew array<System::Int32>(64);

    mnReverse->Tag = 0;

    PanelsArray = gcnew array<System::Windows::Forms::Panel^, 2>(NumCols, NumRows);
    LabelsArray = gcnew array<System::Windows::Forms::Label^, 2>(NumCols, NumRows);

    for (int i = 0; i < NumRows; i++)
        for (int j = 0; j < NumCols; j++)
        {
            PanelsArray[i, j] = (gcnew System::Windows::Forms::Panel());
            PanelsArray[i, j]->Size = System::Drawing::Size(width, width);
            PanelsArray[i, j]->Location = System::Drawing::Point(edge + i * width, top + j * width);
            PanelsArray[i, j]->BackColor = cellColors[(i + j) % 2];
            PanelsArray[i, j]->Tag = i + 8 * j;
            PanelsArray[i, j]->Click += gcnew System::EventHandler(this, &MyForm::OnPanel_Click);

            LabelsArray[i, j] = (gcnew System::Windows::Forms::Label());
            LabelsArray[i, j]->AutoSize = false;
            LabelsArray[i, j]->Size = System::Drawing::Size(width, width);
            LabelsArray[i, j]->Location = System::Drawing::Point(-10, -5);
            LabelsArray[i, j]->Text = "";
            LabelsArray[i, j]->Font = gcnew System::Drawing::Font(chessSymbolsFont, chessFontSize);
            LabelsArray[i, j]->Tag = i + 8 * j;
            LabelsArray[i, j]->Click += gcnew System::EventHandler(this, &MyForm::OnLabel_Click);

            PanelsArray[i, j]->Controls->Add(LabelsArray[i, j]);

            this->Controls->Add(PanelsArray[i, j]);
        }

    labelRankNumbers = gcnew array<System::Windows::Forms::Label^, 2>(2, NumRows);
    labelFileLetters = gcnew array<System::Windows::Forms::Label^, 2>(2, NumCols);

    int fl_width = 10, fl_height = 15;
    int fl_top[] = { top - fl_height, top + NumRows * width };
    int rn_width = 10, rn_height = 15;
    int rn_left[] = { edge - rn_width - 4, edge + NumCols * width };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < NumCols; ++j) {
            labelFileLetters[i, j] = (gcnew System::Windows::Forms::Label());
            labelFileLetters[i, j]->AutoSize = true;
            labelFileLetters[i, j]->Size = System::Drawing::Size(width - 1, width - 1);
            labelFileLetters[i, j]->Location = System::Drawing::Point(edge + j * width + width/2 - fl_width / 2, fl_top[i]);
            labelFileLetters[i, j]->Font = gcnew System::Drawing::Font("Arial", 10);
            labelFileLetters[i, j]->Tag = i * NumCols + j;

            this->Controls->Add(labelFileLetters[i, j]);

            labelRankNumbers[i, j] = (gcnew System::Windows::Forms::Label());
            labelRankNumbers[i, j]->AutoSize = true;
            labelRankNumbers[i, j]->Size = System::Drawing::Size(width - 1, width - 1);
            labelRankNumbers[i, j]->Location = System::Drawing::Point(rn_left[i], top + j * width + width / 2 - rn_height / 2);
            labelRankNumbers[i, j]->Font = gcnew System::Drawing::Font("Arial", 10);
            labelRankNumbers[i, j]->Tag = i * NumCols + j;

            this->Controls->Add(labelRankNumbers[i, j]);
        }
    }
    // Set file letters and rank numbers
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < NumCols; ++j) {
            char let[2] = { 'a' + BoardCol(j), 0 };
            labelFileLetters[i, j]->Text = gcnew System::String(let);
            char num[2] = { '1' + BoardRow(j), 0 };
            labelRankNumbers[i, j]->Text = gcnew System::String(num);
        }
    }
    // Set Normal Game state; start
    mnSetup->Enabled = true;
    panelSetup->Visible = false;
    panelNormal->Visible = true;
    panelNormal->Dock = System::Windows::Forms::DockStyle::Right;
    panelNormal->BringToFront();
    // Set Normal Game state; end

    UpdateBoard(3);

    panelTimes->Width = 100;
    //--------------------------------------------------
    // Open and Save Dialogues
    openFileDialog = gcnew OpenFileDialog();
    openFileDialog->Filter = "PGN Files (*.pgn)|*.pgn|All Files (*.*)|*.*";
    openFileDialog->Title = "Open Chess Game File";
    //--------------------------------------------------
    promotePawnForm = gcnew SharokinChess::PromotePawnForm();

    mnTwoPlayers->Tag = 0;
    bThinking = 0;
    mnNew_Click(nullptr, nullptr);
    board.SetLevel(6, 60 * 1000);

    this->Width = PanelsArray[0, 0]->Left + NumCols * width + PanelsArray[0, 0]->Left + 20 + panelNormal->Width;
    this->Height = top + NumRows * width + 80;
    this->CenterToScreen();
}

void SharokinChess::MyForm::UpdateBoard(int flag)
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            int piece = board.getPieceAt(BoardCol(j), BoardRow(i));
            switch (board.getSideAt(BoardCol(j), BoardRow(i)))
            {
            case SIDE_WHITE:
                ShowPieceOnSquare(j, i, piece, SIDE_WHITE);
                break;
            case SIDE_BLACK:
                ShowPieceOnSquare(j, i, piece, SIDE_BLACK);
                break;
            default:
                ShowPieceOnSquare(j, i, -1, -1);
            }
            PanelsArray[i, j]->BackColor = cellColors[(i + j) % 2];
        }

    int from, to;
    board.getLastMove(&from, &to);
    if (from != -1 && to != -1)
    {
        int OppFromX = BoardCol(from % 8);
        int OppFromY = BoardRow(from / 8);
        int OppToX = BoardCol(to % 8);
        int OppToY = BoardRow(to / 8);
        PanelsArray[OppFromX, OppFromY]->BackColor = moveColors[0];
        PanelsArray[OppToX, OppToY]->BackColor = moveColors[1];
    }
    FromRow = -1;
    FromCol = -1;
    /*
    FindOutCapturedPiece();

    // test
    flag = 3;
    if (flag & 1)
    {
        mmMoves->Lines->Clear();
    }
    */
    /*
    TColor color = TColor(0x00FC7B69);
    char *msg = "";

    switch (board.getGameState())
    {
    case GS_CHECKED:
        color = TColor(0x002D88C6);
        msg = "Check!";
        break;
    case GS_CHECKMATED:
        color = TColor(0x000000CA);
        msg = "Checkmate!";
        break;
    case GS_DRAWBYSTALEMATE:
        msg = "Draw by stalemate!";
        break;
    case GS_DRAWBYTHREEFOLDREPETITION:
        msg = "Draw by three fold repetition!";
        break;
    case GS_DRAWBYFIFTYRULE:
        msg = "Draw by fifty rule!";
        break;
    case GS_DRAWBYINSUFFICIENTMATERIAL:
        msg = "Draw by insufficient material!";
        break;
    case GS_TOOMANYMOVES:
        msg = "Too many moves!";
        break;
    }
    */
    /*
    if (board.getTurn() == SIDE_WHITE)
    {
        lbBlackMsg->Caption = "";
        lbWhiteMsg->Caption = msg;
        lbWhiteMsg->Font->Color = color;
        pnWhiteTurn->Color = clWhite;//color;
        pnBlackTurn->Color = Panel5->Color;
    }
    else
    {
        lbWhiteMsg->Caption = "";
        lbBlackMsg->Caption = msg;
        lbBlackMsg->Font->Color = color;
        pnWhiteTurn->Color = Panel5->Color;
        pnBlackTurn->Color = clBlack;//color;
    }
    */
   
    if (flag & 2)
    {
        richTextMovesHistory->Clear();

        int i = 0;
        char line[50], * str;
        for (; 2 * i < board.getMoveCount(); i++)
        {
            str = line;
            str += sprintf(str, "%d.%s ", i + 1, board.getMoveStr(2 * i));
            str += sprintf(str, "%s   ", board.getMoveStr(2 * i + 1));
            if ((i + 1) % 4 == 0) {
                *str++ = '\n';
                *str++ = '\0';
            }

            richTextMovesHistory->AppendText(gcnew System::String(line));
        }
        HighlightMoveString();
    }
    //if(FromCol != -1)
    //    PanelsArray[FromCol, FromRow]->BackColor = clickColor;

    mnGotoStart->Enabled = board.canStepBack();
    mnStepBack->Enabled = board.canStepBack();
    mnStepForward->Enabled = board.canStepForward();
    mnGotoEnd->Enabled = board.canStepForward();

    UpdateTimes();
    UpdateCapturedPiece();
    Text = "Sharokin Chess - " + savedFileName;
}

void SharokinChess::MyForm::ShowPieceOnSquare(int col, int row, int piece, int side)
{
    if (side == SIDE_WHITE)
    {
        LabelsArray[col, row]->Text = pieces[0, piece];
    }
    else if (side == SIDE_BLACK)
    {
        LabelsArray[col, row]->Text = pieces[1, piece];
    }
    else
        LabelsArray[col, row]->Text = "";
}

void SharokinChess::MyForm::OnSquare_Click(int col, int row)
{
    if (mnSetup->Enabled)
    {   // Normal mode
        if (FromRow == -1)
        {
            FromCol = col;
            FromRow = row;
        }
        else
        {
            PanelsArray[FromCol, FromRow]->BackColor = cellColors[(FromCol + FromRow) % 2];
            if (board.getSideAt(BoardCol(col), BoardRow(row)) == board.getTurn())
            {
                PanelsArray[FromCol, FromRow]->BackColor = cellColors[(FromCol + FromRow) % 2];

                FromCol = col;
                FromRow = row;
            }
            else
            {
                int sx = BoardCol(FromCol), sy = BoardRow(FromRow),
                    dx = BoardCol(col), dy = BoardRow(row);
                int analyse = board.Analyse(sx, sy, dx, dy);

                if (analyse == 0)
                {
                    if (board.getTurn() == SIDE_WHITE)
                        ;// lbWhiteMsg->Caption = "Illegal move!";
                    else
                        ;// lbBlackMsg->Caption = "Illegal move!";
                    FromCol = col;
                    FromRow = row;
                }
                else
                {
                    if (analyse == 2)
                        promotePawnForm->ShowDialog();
                
                    MakeMove();

                    UpdateBoard(3);
                    FromCol = -1;
                    FromRow = -1;
                    mnSave->Enabled = true;
                    if (safe_cast<int>(mnTwoPlayers->Tag) == 0)
                        mnThink_Click(nullptr, nullptr);
                }
            }
        }
        if(FromRow != -1)
            PanelsArray[FromCol, FromRow]->BackColor = clickColor;
    }
    else
    {  // Setup mode
        char* PieceNames[] = { "KQNRBPP", "kqnrbpp" };

        int tag = safe_cast<int>(labelSelectedPiece->Tag);
        if (tag == -1)
        {
            ShowPieceOnSquare(col, row, -1, -1);
            ArrangedPos[BoardCol(col) + BoardRow(row) * 8] = ' ';
        }
        else if (tag < 10)
        {
            ShowPieceOnSquare(col, row, tag % 10, SIDE_WHITE);
            ArrangedPos[BoardCol(col) + BoardRow(row) * 8] = PieceNames[0][tag % 10];
        }
        else
        {
            ShowPieceOnSquare(col, row, tag % 10, SIDE_BLACK);
            ArrangedPos[BoardCol(col) + BoardRow(row) * 8] = PieceNames[1][tag % 10];
        }
    }
}

void SharokinChess::MyForm::OnPanel_Click(System::Object^ sender, System::EventArgs^ e)
{
    System::Windows::Forms::Panel^ clickedPanel = safe_cast<System::Windows::Forms::Panel^>(sender);
    auto position = safe_cast<System::Int32>(clickedPanel->Tag);
    OnSquare_Click(position % 8, position / 8);
}

void SharokinChess::MyForm::OnLabel_Click(System::Object^ sender, System::EventArgs^ e)
{
    System::Windows::Forms::Label^ clickedLabel = safe_cast<System::Windows::Forms::Label^>(sender);
    auto position = safe_cast<System::Int32>(clickedLabel->Tag);
    OnSquare_Click(position % 8, position / 8);
}

void SharokinChess::MyForm::mnSetup_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (false == CloseGame()) {
        return;
    }

    mnSetup->Enabled = false;
    panelNormal->Visible = false;
    panelSetup->Dock = System::Windows::Forms::DockStyle::Right;
    labelSelectedPiece->Text = "";
    labelSelectedPiece->Tag = -1;
    buttonSwitchTurn->Text = "&Turn: White";
    buttonSwitchTurn->Tag = SIDE_WHITE;
    panelSetup->Visible = true;
    panelSetup->BringToFront();

    /*
    Menu = mainMenuArrange;
    mnArrange->Enabled = false;
    sboxArrange->BringToFront();
    Panel8Click(Panel23);
    SwitchTurn(SIDE_WHITE);

    lbWhiteMsg->Caption = "";
    lbBlackMsg->Caption = "";


    for (int i = 0; i < NumCols; i++)
        for (int j = 0; j < NumRows; j++)
        {
            PanelsArray[i][j]->Color = SquarColors[(i + j) % 2];
            switch (board.getSideAt(BoardCol(i), BoardRow(j)))
            {
            case SIDE_WHITE:
                ArrangedPos[BoardCol(i) + 8 * BoardRow(j)] =
                    PieceNames[0][board.getPieceAt(BoardCol(i), BoardRow(j))];
                break;
            case SIDE_BLACK:
                ArrangedPos[BoardCol(i) + 8 * BoardRow(j)] =
                    PieceNames[1][board.getPieceAt(BoardCol(i), BoardRow(j))];
                break;
            default:
                ArrangedPos[BoardCol(i) + 8 * BoardRow(j)] = ' ';
            }
        }
    */
}

void SharokinChess::MyForm::buttonCancelSetup_Click(System::Object^ sender, System::EventArgs^ e)
{
    // Set Normal Game state; start
    mnSetup->Enabled = true;
    panelSetup->Visible = false;
    panelNormal->Visible = true;
    panelNormal->Dock = System::Windows::Forms::DockStyle::Right;
    panelNormal->BringToFront();
    // Set Normal Game state; end
}

void SharokinChess::MyForm::mnThink_Click(System::Object^ sender, System::EventArgs^ e)
{
    //Menu = MainMenu2;
    this->MainMenuStrip = this->menuStripStop;
    this->Controls->Remove(this->menuStripMain);
    this->Controls->Add(this->menuStripStop);
    board.Think();
    bThinking = 1;
}

void SharokinChess::MyForm::mnTwoPlayers_Click(System::Object^ sender, System::EventArgs^ e)
{
    mnTwoPlayers->Tag = 1 - safe_cast<int>(mnTwoPlayers->Tag);
    mnTwoPlayers->Text =
        mnTwoPlayers->Tag ? "&Single player" : "&Two players";
}

void SharokinChess::MyForm::buttonSetupPiece_Click(System::Object^ sender, System::EventArgs^ e)
{
    auto sdr = safe_cast<System::Windows::Forms::Button^>(sender);
    int piece_type = safe_cast<int>(sdr->Tag);
    labelSelectedPiece->Tag = piece_type;
    if (piece_type == -1) {
        labelSelectedPiece->Text = "X";
    }
    else {
        labelSelectedPiece->Text = pieces[piece_type / 10, piece_type % 10];
    }
}

void SharokinChess::MyForm::buttonSwitchTurn_Click(System::Object^ sender, System::EventArgs^ e)
{
    auto sdr = safe_cast<System::Windows::Forms::Button^>(sender);
    int turn = safe_cast<int>(sdr->Tag);
    if (turn == SIDE_WHITE) {
        buttonSwitchTurn->Text = "&Turn: Black";
        buttonSwitchTurn->Tag = SIDE_BLACK;
    }
    else {
        buttonSwitchTurn->Text = "&Turn: White";
        buttonSwitchTurn->Tag = SIDE_WHITE;
    }
}

void SharokinChess::MyForm::mnReverse_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (safe_cast<int>(mnReverse->Tag) == 0)
    {
        mnReverse->Tag = 1;

        char num[] = {'1', 0};
        for (int i = 0; i < NumRows; i++)
        {
            num[0] = '1' + BoardRow(i);
            labelRankNumbers[0, i]->Text = gcnew System::String(num);
            labelRankNumbers[1, i]->Text = gcnew System::String(num);
        }

        char let[] = { 'a', 0 };
        for (int i = 0; i < NumCols; i++)
        {
            let[0] = 'a' + BoardCol(i);
            labelFileLetters[0, i]->Text = gcnew System::String(let);
            labelFileLetters[1, i]->Text = gcnew System::String(let);
        }
        /*
        int top = pnBlackSide->Top;
        pnBlackSide->Top = pnWhiteSide->Top;
        pnWhiteSide->Top = top;

        pnWhiteTurn->Top = 0;
        pnBlackTurn->Top = pnBlackSide->Height - pnBlackTurn->Height;

        top = pnBlackTime->Top;
        pnBlackTime->Top = pnWhiteTime->Top;
        pnWhiteTime->Top = top;

        for (int i = 0; i < 8; i++)
        {
            top = CapturedImages[0][i]->Top;
            CapturedImages[0][i]->Top = CapturedImages[1][i]->Top;
            CapturedImages[1][i]->Top = top;
        }*/
    }
    else
    {
        mnReverse->Tag = 0;

        char num[] = { '1', 0 };
        for (int i = 0; i < NumRows; i++)
        {
            num[0] = '1' + BoardRow(i);
            labelRankNumbers[0, i]->Text = gcnew System::String(num);
            labelRankNumbers[1, i]->Text = gcnew System::String(num);
        }
        char let[] = { 'a', 0 };
        for (int i = 0; i < NumCols; i++)
        {
            let[0] = 'a' + BoardCol(i);
            labelFileLetters[0, i]->Text = gcnew System::String(let);
            labelFileLetters[1, i]->Text = gcnew System::String(let);
        }
        /*
        int top = pnBlackSide->Top;
        pnBlackSide->Top = pnWhiteSide->Top;
        pnWhiteSide->Top = top;

        pnWhiteTurn->Top = pnWhiteSide->Height - pnWhiteTurn->Height;;
        pnBlackTurn->Top = 0;

        top = pnBlackTime->Top;
        pnBlackTime->Top = pnWhiteTime->Top;
        pnWhiteTime->Top = top;

        for (int i = 0; i < 8; i++)
        {
            top = CapturedImages[0][i]->Top;
            CapturedImages[0][i]->Top = CapturedImages[1][i]->Top;
            CapturedImages[1][i]->Top = top;
        }*/
    }
    UpdateBoard(0);
}

void SharokinChess::MyForm::buttonSetupDone_Click(System::Object^ sender, System::EventArgs^ e)
{
    int castle = 0;
    //if (chbWhiteShortCastle->Checked)
    //    castle |= 1;
    //if (chbWhiteLongCastle->Checked)
    //    castle |= 2;
    //if (chbBlackShortCastle->Checked)
    //    castle |= 4;
    //if (chbBlackLongCastle->Checked)
    //    castle |= 8;
    char ap[64];
    for (int i = 0; i < 64; ++i) {
        ap[i] = ArrangedPos[i];
    }
    if (!board.AnalysePosition(ap, safe_cast<int>(buttonSwitchTurn->Tag), "", castle))
    {
        /*
        Menu = MainMenu1;
        pnWhiteSide->Visible = true;
        pnBlackSide->Visible = true;

        FileName = "";
        mnSave->Enabled = true;
        Caption = AnsiString("Sharokin Chess - ");
        */
        board.SetupBoard();
        savedFileName = "";
        this->Text = "Sharokin Chess - " + savedFileName;
        // Set Normal Game state; start
        mnSetup->Enabled = true;
        panelSetup->Visible = false;
        panelNormal->Visible = true;
        panelNormal->Dock = System::Windows::Forms::DockStyle::Right;
        panelNormal->BringToFront();
        // Set Normal Game state; end

        UpdateBoard(1);
    }
    //else
    //    MessageBox(Handle, "Illegal position", "Error", MB_OK);
}

void SharokinChess::MyForm::buttonClearBoard_Click(System::Object^ sender, System::EventArgs^ e)
{
    for (int i = 0; i < NumCols; i++)
        for (int j = 0; j < NumRows; j++)
        {
            ShowPieceOnSquare(j, i, -1, -1);
            ArrangedPos[i + 8 * j] = ' ';
        }
}

void SharokinChess::MyForm::UpdateTimes()
{
    long time = board.getTime(SIDE_WHITE) / 1000;
    char str_time[10];
    {
        sprintf(str_time, "%02d:%02d:%02d", time / 3600, (time % 3600) / 60, (time % 3600) % 60);
        labelWhiteTime->Text = gcnew System::String(str_time);
    }
    time = board.getTime(SIDE_BLACK) / 1000;
    sprintf(str_time, "%02d:%02d:%02d", time / 3600, (time % 3600) / 60, (time % 3600) % 60);
    labelBlackTime->Text = gcnew System::String(str_time);
}

bool alreadyProcessed = false;

void SharokinChess::MyForm::OnAppIdle(System::Object^ sender, System::EventArgs^ e)
{
    //if (board.IsThinking())
    //    Sleep(100);

    if (alreadyProcessed)
        return;
    alreadyProcessed = true;

    if (bThinking)
        if (board.ThinkFinished())
        {
            MakeMove();

            //board.MakeMove();
            //long time = Board.getLastMoveTime();
            //lbLastMoveTime-> Caption = AnsiString(time / 1000) + AnsiString(".") + AnsiString(time % 1000);
            //lbGenMovesCount->Caption = Board.getGenMovesCount();
            //lbMakeMoveCount->Caption = Board.getMakeMoveCount();
            //lbNodesCount->Caption    = Board.getNodesCount();
            UpdateBoard(3);
            mnSave->Enabled = true;
            this->MainMenuStrip = this->menuStripMain;
            this->Controls->Remove(this->menuStripStop);
            this->Controls->Add(this->menuStripMain);
            //Menu = MainMenu1;
            bThinking = 0;
        }

    //auto start_x = steady_clock::now();
    /*
    if (GetTickCount() >= time0 + 1000)
    {
        time0 = GetTickCount();
        UpdateTimes();
    }
    */
    alreadyProcessed = false;
}

void SharokinChess::MyForm::WhiteTimer_Tick(System::Object^ sender, System::EventArgs^ e)
{
    whiteSeconds++;
    labelWhiteTime->Text = String::Format("{0:D2}:{1:D2}:{2:D2}", whiteSeconds / 3600, (whiteSeconds % 3600) / 60, whiteSeconds % 60);
}

void SharokinChess::MyForm::BlackTimer_Tick(System::Object^ sender, System::EventArgs^ e)
{
    blackSeconds++;
    labelBlackTime->Text = String::Format("{0:D2}:{1:D2}:{2:D2}", blackSeconds / 3600, (blackSeconds % 3600) / 60, blackSeconds % 60);
}

void SharokinChess::MyForm::mnOpen_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        if (false == CloseGame()) {
            return;
        }

        String^ filePath = openFileDialog->FileName;
        const char* fileName = static_cast<const char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filePath).ToPointer() );
        if (board.Open(fileName))
        {
            MessageBox::Show("Couldn't open the file:\n" + filePath);
        }
        else
        {
            switch (board.getGameState()) {
            case GS_CHECKMATED:
            case GS_DRAWBYFIFTYRULE:
            case GS_DRAWBYSTALEMATE:
            case GS_DRAWBYTHREEFOLDREPETITION:
            case GS_DRAWBYINSUFFICIENTMATERIAL:
                mnStepBack->Enabled = true;
                mnStepForward->Enabled = true;
                //break;
            //default:
                //mnStepBack->Enabled = false;
                //mnStepForward->Enabled = false;
                break;
            }
            UpdateBoard(1 | 2);
            mnSave->Enabled = false;
            savedFileName = openFileDialog->FileName;
            this->Text = "Sharokin Chess - " + savedFileName;
        }
    }
}

void SharokinChess::MyForm::mnSave_Click(System::Object^ sender, System::EventArgs^ e)
{
    SaveGame(false);
}

void SharokinChess::MyForm::mnSaveAs_Click(System::Object^ sender, System::EventArgs^ e)
{
    SaveGame(true);
}

bool SharokinChess::MyForm::SaveGame(bool showSaveDialog)
{
    using namespace System::Runtime::InteropServices;
    if (true == showSaveDialog || System::String::IsNullOrEmpty(savedFileName)) {
        if (false == ShowSaveDialog()) {
            return false;
        }
    }

    IntPtr ptr = Marshal::StringToHGlobalAnsi(savedFileName);
    const char* fileName = static_cast<const char*>(ptr.ToPointer());
    if (board.Save(fileName))
        ;//MessageBox(Handle, "Can not save to file!", "", MB_OK);
    else
    {
        mnSave->Enabled = false;
        this->Text = "Sharokin Chess - " + savedFileName;
    }
    System::Runtime::InteropServices::Marshal::FreeHGlobal(ptr);
    return true;
}

bool SharokinChess::MyForm::ShowSaveDialog()
{
    using namespace System::Runtime::InteropServices;
    if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        savedFileName = saveFileDialog->FileName;
        return true;
    }
    else {
        return false;
    }
}

bool SharokinChess::MyForm::CloseGame()
{
    if (mnSave->Enabled) {
        using namespace System::Windows::Forms;
        switch (MessageBox::Show("    Game not saved!\n    Do you want to save it?",
            "Confirm",
            MessageBoxButtons::YesNoCancel,
            MessageBoxIcon::Exclamation,
            MessageBoxDefaultButton::Button1))
        {
        case System::Windows::Forms::DialogResult::Yes:
            if (false == SaveGame(false)) {
                return false;
            }
        case System::Windows::Forms::DialogResult::No:
            return true;
        case System::Windows::Forms::DialogResult::Cancel:
            return false;
        }
    }
    return true;
}

void SharokinChess::MyForm::mnExit_Click(System::Object^ sender, System::EventArgs^ e)
{
    Close();
}

void SharokinChess::MyForm::MakeMove()
{
    if (board.getTurn() == SIDE_WHITE) {
        whiteTimer->Stop();
        board.MakeMove();
        blackTimer->Start();
    }
    else {
        blackTimer->Stop();
        board.MakeMove();
        whiteTimer->Start();
    }
    UpdateCapturedPiece();
}

void SharokinChess::MyForm::mnNew_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (false == CloseGame()) {
        return;
    }
    
    whiteTimer->Stop();
    blackTimer->Stop();
    whiteSeconds = 0;
    blackSeconds = 0;
    {
        board.SetupBoard();
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                PanelsArray[j, i]->Text = "";
        FromRow = -1;
        FromCol = -1;
        savedFileName = "";
        UpdateBoard(3);
        mnSave->Enabled = false;
        //lbWhiteMsg->Font->Color = clBlack;
        //lbBlackMsg->Font->Color = clBlack;
    }
    labelWhitePieces->Text = "";
    labelBlackPieces->Text = "";
    //mnStepBack->Enabled = false;
    //mnStepForward->Enabled = false;
}

void SharokinChess::MyForm::mnStop_Click(System::Object^ sender, System::EventArgs^ e)
{
    board.StopThinking();
}

void SharokinChess::MyForm::UpdateCapturedPiece()
{
    int p_Count[2][6], std_num_pieces[] = { 1, 1, 2, 2, 2, 8 };

    labelWhitePieces->Text = "";
    labelBlackPieces->Text = "";

    board.CountPieces(0, p_Count[0]);
    board.CountPieces(1, p_Count[1]);

    // ----------------------------------------------------
    int order[] = {PIECE_KING, PIECE_QUEEN, PIECE_ROOK, PIECE_BISHOP, PIECE_KNIGHT, PIECE_PAWN(SIDE_WHITE)};
    for (int i = 0; i < 6; i++) {
        int p = order[i];
        /*
        int d = p_Count[SIDE_WHITE][p] - p_Count[SIDE_BLACK][p];
        if (d > 0) {
            for (int j = 0; j < d; ++j) {
                labelWhitePieces->Text += pieces[SIDE_WHITE, p];
            }
        }
        else if (d < 0) {
            for (int j = 0; j < -d; ++j) {
                labelBlackPieces->Text += pieces[SIDE_BLACK, p];
            }
        }
        */
        for (int j = p_Count[SIDE_WHITE][p]; j < std_num_pieces[p]; ++j) {
            labelBlackPieces->Text += pieces[SIDE_WHITE, p];
        }
        for (int j = p_Count[SIDE_BLACK][p]; j < std_num_pieces[p]; ++j) {
            labelWhitePieces->Text += pieces[SIDE_BLACK, p];
        }
    }
}

void SharokinChess::MyForm::HighlightMoveString()
{
    int head = board.getMoveHead() - 1;
    if (head < 0) {
        richTextMovesHistory->Select(0, 0);
    }
    else if (head % 2 == 1) {
        char text[10];
        snprintf(text, sizeof(text), "%d.", (head + 1) / 2);
        int start = richTextMovesHistory->Find(gcnew System::String(text)), end;
        start = richTextMovesHistory->Find(gcnew System::String(" "), start, System::Windows::Forms::RichTextBoxFinds::None) + 1;
        end = richTextMovesHistory->Find(gcnew System::String(" "), start, System::Windows::Forms::RichTextBoxFinds::None);
        richTextMovesHistory->Select(start, end - start);
    }
    else {
        char text[10];
        snprintf(text, sizeof(text), "%d.", (head + 2) / 2);
        int start = richTextMovesHistory->Find(gcnew System::String(text)), end;
        start = richTextMovesHistory->Find(gcnew System::String("."), start, System::Windows::Forms::RichTextBoxFinds::None) + 1;
        end = richTextMovesHistory->Find(gcnew System::String(" "), start, System::Windows::Forms::RichTextBoxFinds::None);
        richTextMovesHistory->Select(start, end - start);
    }
}

void SharokinChess::MyForm::mnStepBack_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (!board.stepBack()) {
        UpdateBoard(2);
    }
}

void SharokinChess::MyForm::mnStepForward_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (!board.stepForward()) {
        UpdateBoard(2);
    }
}

#include <msclr/marshal_cppstd.h>
void SharokinChess::MyForm::richTextMovesHistory_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
    int head = -1;
    int s = richTextMovesHistory->SelectionStart;
    if (s > 0) {
        std::string moves = msclr::interop::marshal_as<std::string>(richTextMovesHistory->Text);
        int pos = moves.substr(0, s).find_last_of(".");
        if (pos != std::string::npos) {
            int one = (s > pos && moves.substr(pos, s - pos).find_first_of(" ") != std::string::npos ? 1 : 0);
            int space = moves.substr(0, pos).find_last_of(" ");
            if (space == std::string::npos)
                space = 0;
            else
                ++space;
            head = 2 * (std::stoi(moves.substr(space)) - 1) + one;
        }
    }
    while ( (head == -1 ? 0 : head) < board.getMoveHead()) {
        board.stepBack();
    }
    while (head >= board.getMoveHead()) {
        if (board.stepForward() == 1)
            break;
    }
    UpdateBoard(3);
}
