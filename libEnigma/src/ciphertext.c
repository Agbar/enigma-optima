/*
 * This file is part of enigma-suite-0.76, which is distributed under the terms
 * of the General Public License (GPL), version 2. See doc/COPYING for details.
 *
 * Copyright (C) 2005 Stefan Krah
 *
 */

#include "ciphertext.h"

// Limit imposed by highest value in dictionary 71964 (bidict.naval) and size
// of scoring variable (32-bit signed) gives limit of ciphertext
// length of about 29800 (very pessimistically).
// I assume that 2048 characters should be enough in any realistic scenario,
// especially that message is trimmed to 250 characters (CT in global.h).
// Allegedly there exist messages longer than 1000 chars.
union ciphertext_t ciphertext;
