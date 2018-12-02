#!/usr/bin/env bash
set -e -o pipefail
l="$(cat)"
twos=0
threes=0
while IFS= read -r line; do
	counts="$(sed -e 's/./&\n/g' <<< "$line" | sort | uniq -c |
		sed -e 's/^ *\([0-9]\+\) *.*$/\1/')"
	if grep -q '^2$' <<< "$counts"; then ((twos++)) || true; fi
	if grep -q '^3$' <<< "$counts"; then ((threes++)) || true; fi
done <<< "$l"
printf '%d\n' "$((twos*threes))"

i=0
while IFS= read -r a; do
	out="$(tail -n +$i <<< "$l" |
		while IFS= read -r b; do
			diff=0
			s=
			for ((j=0; j<${#a}; j++)); do
				if [[ "${a:$j:1}" != "${b:$j:1}" ]]; then
					((diff++)) || true
				else
					s="$s${a:$j:1}"
				fi
			done
			if ((diff == 1)); then
				printf '%s' "$s"
			fi
		done)"
	if [[ -n "$out" ]]; then
		printf '%s\n' "$out"
		exit
	fi
	((i++)) || true
done <<< "$l"
