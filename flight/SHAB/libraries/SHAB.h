/*
  SHAB.h contains all declarations for in-house SHAB code
  Copyright (C) 2018  Alex Hyer, Andrew Jong
 
  This file is part of SHAB.

  SHAB is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  SHAB is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with SHAB.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SHAB_SHAB_H_
#define SHAB_SHAB_H_

class LinearActuator {
  private:
    bool extended = false;
    int fpin, rpin;
    
  public:
    LinearActuator (int f, int r);
    bool get_extended();
};

#endif // SHAB_SHAB_H_
