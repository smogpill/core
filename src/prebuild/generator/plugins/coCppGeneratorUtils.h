// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coStringOutputStream;
class coConstString;

void coWriteInclude(coStringOutputStream& _stream, const coConstString& _path);
void coWriteHeader(coStringOutputStream& _stream);