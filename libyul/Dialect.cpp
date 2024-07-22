/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0
/**
 * Yul dialect.
 */

#include <libyul/Dialect.h>
#include <libyul/AST.h>

using namespace solidity::yul;
using namespace solidity::langutil;

Literal Dialect::zeroLiteralForType(solidity::yul::YulName _type) const
{
	if (_type == boolType && _type != defaultType)
		return {DebugData::create(), LiteralKind::Boolean, LiteralValue(false), _type};
	return {DebugData::create(), LiteralKind::Number, LiteralValue(0, std::nullopt), _type};
}


Literal Dialect::trueLiteral() const
{
	if (boolType != defaultType)
		return {DebugData::create(), LiteralKind::Boolean, LiteralValue(true), boolType};
	else
		return {DebugData::create(), LiteralKind::Number, LiteralValue(1), defaultType};
}

bool Dialect::validTypeForLiteral(LiteralKind _kind, LiteralValue const&, YulName _type) const
{
	if (_kind == LiteralKind::Boolean)
		return _type == boolType;
	else
		return true;
}

Dialect const& Dialect::yulDeprecated()
{
	static std::unique_ptr<Dialect> dialect;
	static YulStringRepository::ResetCallback callback{[&] { dialect.reset(); }};

	if (!dialect)
	{
		// TODO will probably change, especially the list of types.
		dialect = std::make_unique<Dialect>();
		dialect->defaultType = "u256"_yulname;
		dialect->boolType = "bool"_yulname;
		dialect->types = {
			"bool"_yulname,
			"u8"_yulname,
			"s8"_yulname,
			"u32"_yulname,
			"s32"_yulname,
			"u64"_yulname,
			"s64"_yulname,
			"u128"_yulname,
			"s128"_yulname,
			"u256"_yulname,
			"s256"_yulname};
	};

	return *dialect;
}
