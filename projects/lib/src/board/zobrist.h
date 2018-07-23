/*
    This file is part of Cute Chess.
    Copyright (C) 2008-2018 Cute Chess authors

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

#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <QtGlobal>

namespace Chess {
class Piece;

/*!
 * \brief Unsigned 64-bit values for generating zobrist position keys.
 *
 * Chess::Board uses zobrist keys to quickly and easily compare two
 * positions for equality. Primary uses for zobrist keys are:
 * - Detecting repetitions, ie. a draw by three-fold repetition
 * - Opening books
 * - In hash table entries
 */
class LIB_EXPORT Zobrist
{
	public:
		/*!
		 * Creates a new uninitialized Zobrist object.
		 *
		 * \param keys An array of zobrist keys that can be used
		 * instead of the random numbers generated by the Zobrist
		 * class.
		 */
		Zobrist(const quint64* keys = nullptr);
		/*! Destroys the Zobrist object. */
		virtual ~Zobrist() {}

		/*! Returns true if the keys are initialized. */
		bool isInitialized() const;
		/*!
		 * Initializes the zobrist numbers.
		 *
		 * \param squareCount The number of squares the board has,
		 * including the invisible "Wall" squares.
		 * \param pieceTypeCount The number of piece types the variant
		 * has, including the empty "NoPiece" type (type 0).
		 *
		 * \note Subclasses that reimplement this function must call
		 * the base implementation.
		 */
		virtual void initialize(int squareCount,
					int pieceTypeCount);

		/*!
		 * Returns the zobrist value for side to move.
		 * This value must be in the key on black's turn.
		 */
		virtual quint64 side() const;
		/*! Returns the zobrist value for \a piece at \a square. */
		virtual quint64 piece(const Piece& piece, int square) const;
		/*!
		 * Returns the zobrist value for reserve piece \a piece at \a slot.
		 *
		 * \note \a slot is zero-based, so the first piece of type
		 * \a piece is at slot 0.
		 */
		virtual quint64 reservePiece(const Piece& piece, int slot) const;

	protected:
		/*!
		 * Returns the number of squares the board has, including the
		 * invisible "Wall" squares.
		 */
		int squareCount() const;
		/*!
		 * Returns the number of piece types the variant has, including
		 * the empty "NoPiece" type (type 0).
		 */
		int pieceTypeCount() const;
		/*! Returns the array of zobrist keys. */
		const quint64* keys() const;

		/*! Returns an unsigned 64-bit pseudo-random number. */
		static quint64 random64();

	private:
		static int random32();
		static int s_randomSeed;

		bool m_initialized;
		int m_squareCount;
		int m_pieceTypeCount;
		const quint64* m_keys;
};

inline int Zobrist::squareCount() const
{
	return m_squareCount;
}

inline int Zobrist::pieceTypeCount() const
{
	return m_pieceTypeCount;
}

inline const quint64* Zobrist::keys() const
{
	return m_keys;
}

} //namespace Chess
#endif // ZOBRIST
