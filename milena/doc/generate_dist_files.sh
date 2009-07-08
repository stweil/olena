# Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

#! /bin/sh

## FIXME: Don't use `echo -n', as echo's options are not portable.
##
## http://www.gnu.org/software/autoconf/manual/html_node/Limitations-of-Builtins.html#Limitations-of-Builtins
##
## As an ugly workaround, use `printf' instead.  The right approach
## would be to only emit lines ended with newlines.
ECHO_N=printf

# Use the C locale to have a deterministic sort.
export LC_ALL=C

if [ $# -ne 2 ]; then
  echo "Usage: $0 <path to scan> <filename filter>" 1>&2
  exit 1
fi

OUT="$1/$1.mk"
echo "## Generated by ../generate_dist_files, do not modify." > $OUT
echo >>$OUT

$ECHO_N "EXTRA_DIST += " >> $OUT
for ftype in $2; do
  for file in `find $1 -name *.$ftype | sort`; do
    echo " \\" >> $OUT
    $ECHO_N $file >> $OUT
  done
done

echo "" >> $OUT
