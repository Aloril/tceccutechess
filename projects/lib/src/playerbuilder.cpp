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

#include "playerbuilder.h"

PlayerBuilder::PlayerBuilder(const QString& name)
	: m_name(name),
	  m_rating(0),
	  m_resume_score(0),
	  m_strikes(0)
{
}

PlayerBuilder::~PlayerBuilder()
{
}

QString PlayerBuilder::name() const
{
	return m_name;
}

void PlayerBuilder::setName(const QString& name)
{
	m_name = name;
}

int PlayerBuilder::rating() const
{
	return m_rating;
}

void PlayerBuilder::setRating(const int rating)
{
	m_rating = rating;
}

int PlayerBuilder::strikes() const
{
	return m_strikes;
}

void PlayerBuilder::setStrikes(const int strikes)
{
	m_strikes = strikes;
}

void PlayerBuilder::setResumeScore(const int score)
{
   m_resume_score = score;
}

int PlayerBuilder::resumescore() const
{
   return m_resume_score;
}

