/*
    This file is part of Cute Chess.

    Cute Chess is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cute Chess is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cute Chess.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "crazyhouseboard.h"
#include "westernzobrist.h"

namespace Chess {

CrazyhouseBoard::CrazyhouseBoard(QObject* parent)
	: WesternBoard(new WesternZobrist(), parent)
{
	setPieceType(PromotedKnight, tr("promoted knight"), "N~", KnightMovement);
	setPieceType(PromotedBishop, tr("promoted bishop"), "B~", BishopMovement);
	setPieceType(PromotedRook, tr("promoted rook"), "R~", RookMovement);
	setPieceType(PromotedQueen, tr("promoted queen"), "Q~", BishopMovement | RookMovement);
}

QString CrazyhouseBoard::variant() const
{
	return "crazyhouse";
}

bool CrazyhouseBoard::variantHasDrops() const
{
	return true;
}

QString CrazyhouseBoard::defaultFenString() const
{
	return "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - KQkq - 0 1";
}

int CrazyhouseBoard::handPieceType(int pieceType) const
{
	if (pieceType >= PromotedKnight && pieceType <= PromotedQueen)
		return Pawn;
	return pieceType;
}

int CrazyhouseBoard::normalPieceType(int type)
{
	switch (type)
	{
	case PromotedKnight:
		return Knight;
	case PromotedBishop:
		return Bishop;
	case PromotedRook:
		return Rook;
	case PromotedQueen:
		return Queen;
	default:
		return type;
	}
}

int CrazyhouseBoard::promotedPieceType(int type)
{
	switch (type)
	{
	case Knight:
		return PromotedKnight;
	case Bishop:
		return PromotedBishop;
	case Rook:
		return PromotedRook;
	case Queen:
		return PromotedQueen;
	default:
		return type;
	}
}

void CrazyhouseBoard::normalizePieces(Piece piece, QVarLengthArray<int>& squares)
{
	if (!piece.isValid())
		return;

	Piece prom(piece.side(), promotedPieceType(piece.type()));
	Piece base(piece.side(), normalPieceType(piece.type()));
	if (base == prom)
		return;

	const int size = arraySize();
	for (int i = 0; i < size; i++)
	{
		if (pieceAt(i) == prom)
		{
			squares.append(i);
			setSquare(i, base);
		}
	}
}

void CrazyhouseBoard::restorePieces(Piece piece, const QVarLengthArray<int>& squares)
{
	if (!piece.isValid() || squares.isEmpty())
		return;

	Piece prom(piece.side(), promotedPieceType(piece.type()));
	for (int i = 0; i < squares.size(); i++)
		setSquare(squares[i], prom);
}

QString CrazyhouseBoard::sanMoveString(const Move& move)
{
	Piece piece(pieceAt(move.sourceSquare()));
	QVarLengthArray<int> squares;

	normalizePieces(piece, squares);
	QString str(WesternBoard::sanMoveString(move));
	restorePieces(piece, squares);

	return str;
}

Move CrazyhouseBoard::moveFromSanString(const QString& str)
{
	if (str.isEmpty())
		return Move();

	Piece piece(pieceFromSymbol(str.at(0)));
	if (piece.isValid())
	{
		piece.setSide(sideToMove());
		QVarLengthArray<int> squares;

		normalizePieces(piece, squares);
		Move move(WesternBoard::moveFromSanString(str));
		restorePieces(piece, squares);

		return move;
	}

	return WesternBoard::moveFromSanString(str);
}

void CrazyhouseBoard::vMakeMove(const Move& move, QVarLengthArray<int>& changedSquares)
{
	Q_UNUSED(changedSquares);

	int source = move.sourceSquare();
	int target = move.targetSquare();
	int prom = move.promotion();

	Move tmp(move);
	if (source != 0 && prom != Piece::NoPiece)
		tmp = Move(source, target, promotedPieceType(prom));
	
	int ctype = captureType(move);
	if (ctype != Piece::NoPiece)
		addHandPiece(Piece(sideToMove(), handPieceType(ctype)));
	else if (source == 0)
		removeHandPiece(Piece(sideToMove(), prom));

	return WesternBoard::vMakeMove(tmp, changedSquares);
}

void CrazyhouseBoard::vUndoMove(const Move& move)
{
	int source = move.sourceSquare();
	int target = move.targetSquare();
	int prom = move.promotion();

	Move tmp(move);
	if (source != 0 && prom != Piece::NoPiece)
		tmp = Move(source, target, promotedPieceType(prom));

	WesternBoard::vUndoMove(tmp);

	int ctype = captureType(move);
	if (ctype != Piece::NoPiece)
		removeHandPiece(Piece(sideToMove(), handPieceType(ctype)));
	else if (source == 0)
		addHandPiece(Piece(sideToMove(), prom));
}

void CrazyhouseBoard::generateMovesForPiece(QVarLengthArray<Move>& moves,
					    int pieceType,
					    int square) const
{
	// Generate drops
	if (square == 0)
	{
		const int size = arraySize();
		const int maxRank = height() - 2;
		for (int i = 0; i < size; i++)
		{
			Piece tmp = pieceAt(i);
			if (!tmp.isEmpty())
				continue;
			if (pieceType == Pawn)
			{
				Square sq(chessSquare(i));
				if (sq.rank() < 1 || sq.rank() > maxRank)
					continue;
			}

			moves.append(Move(0, i, pieceType));
		}
	}
	else
		WesternBoard::generateMovesForPiece(moves, pieceType, square);
}

} // namespace Chess