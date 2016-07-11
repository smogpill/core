// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coChar_f.h"

#define _coCC(_category_) _coCharCategory_##_category_

//-------------------------------------------------------
const coUint16 _coCharCategoryTable[256] =
{
	// 0 NUL         1 SOH         2 STX         3 ETX
	// 4 EOT         5 ENQ         6 ACK         7 BEL
	0, 0, 0, 0,
	0, 0, 0, 0,
	// 8 BS          9 HT         10 NL         11 VT
	//12 NP         13 CR         14 SO         15 SI
	0, _coCC(horzws), _coCC(vertws), _coCC(horzws),
	_coCC(horzws), _coCC(vertws), 0, 0,
	//16 DLE        17 DC1        18 DC2        19 DC3
	//20 DC4        21 NAK        22 SYN        23 ETB
	0, 0, 0, 0,
	0, 0, 0, 0,
	//24 CAN        25 EM         26 SUB        27 ESC
	//28 FS         29 GS         30 RS         31 US
	0, 0, 0, 0,
	0, 0, 0, 0,
	//32 SP         33  !         34  "         35  #
	//36  $         37  %         38  &         39  '
	_coCC(space), _coCC(rawdel), _coCC(rawdel), _coCC(rawdel),
	_coCC(punct), _coCC(rawdel), _coCC(rawdel), _coCC(rawdel),
	//40  (         41  )         42  *         43  +
	//44  ,         45  -         46  .         47  /
	_coCC(punct), _coCC(punct), _coCC(rawdel), _coCC(rawdel),
	_coCC(rawdel), _coCC(rawdel), _coCC(period), _coCC(rawdel),
	//48  0         49  1         50  2         51  3
	//52  4         53  5         54  6         55  7
	_coCC(digit), _coCC(digit), _coCC(digit), _coCC(digit),
	_coCC(digit), _coCC(digit), _coCC(digit), _coCC(digit),
	//56  8         57  9         58  :         59  ;
	//60  <         61  =         62  >         63  ?
	_coCC(digit), _coCC(digit), _coCC(rawdel), _coCC(rawdel),
	_coCC(rawdel), _coCC(rawdel), _coCC(rawdel), _coCC(rawdel),
	//64  @         65  A         66  B         67  C
	//68  D         69  E         70  F         71  G
	_coCC(punct), _coCC(xupper), _coCC(xupper), _coCC(xupper),
	_coCC(xupper), _coCC(xupper), _coCC(xupper), _coCC(upper),
	//72  H         73  I         74  J         75  K
	//76  L         77  M         78  N         79  O
	_coCC(upper), _coCC(upper), _coCC(upper), _coCC(upper),
	_coCC(upper), _coCC(upper), _coCC(upper), _coCC(upper),
	//80  P         81  Q         82  R         83  S
	//84  T         85  U         86  V         87  W
	_coCC(upper), _coCC(upper), _coCC(upper), _coCC(upper),
	_coCC(upper), _coCC(upper), _coCC(upper), _coCC(upper),
	//88  X         89  Y         90  Z         91  [
	//92  \         93  ]         94  ^         95  _
	_coCC(upper), _coCC(upper), _coCC(upper), _coCC(rawdel),
	_coCC(punct), _coCC(rawdel), _coCC(rawdel), _coCC(under),
	//96  `         97  a         98  b         99  c
	//100  d       101  e        102  f        103  g
	_coCC(punct), _coCC(xlower), _coCC(xlower), _coCC(xlower),
	_coCC(xlower), _coCC(xlower), _coCC(xlower), _coCC(lower),
	//104  h       105  i        106  j        107  k
	//108  l       109  m        110  n        111  o
	_coCC(lower), _coCC(lower), _coCC(lower), _coCC(lower),
	_coCC(lower), _coCC(lower), _coCC(lower), _coCC(lower),
	//112  p       113  q        114  r        115  s
	//116  t       117  u        118  v        119  w
	_coCC(lower), _coCC(lower), _coCC(lower), _coCC(lower),
	_coCC(lower), _coCC(lower), _coCC(lower), _coCC(lower),
	//120  x       121  y        122  z        123  {
	//124  |       125  }        126  ~        127 DEL
	_coCC(lower), _coCC(lower), _coCC(lower), _coCC(rawdel),
	_coCC(rawdel), _coCC(rawdel), _coCC(rawdel), 0
};
