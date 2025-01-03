import re
import sys

data = """EXTRA { display-name = "Extra"; bindings = < &none  &kp Q  &kp W  &kp E  &kp R  &kp T       &kp Y  &kp U  &kp I  &kp O  &kp P  &none &none  &u_mt LGUI A  &u_mt LALT S  &u_mt LCTRL D  &u_mt LSHFT F  &kp G       &kp H  &u_mt LSHFT J  &u_mt LCTRL K  &u_mt LALT L  &u_mt LGUI SQT  &none &none  &u_lt 3 Z  &u_mt RALT X  &kp C  &kp V  &kp B       &kp N  &kp M  &kp COMMA  &u_mt RALT DOT  &u_lt 3 SLASH  &none &u_lt 6 ESC  &u_lt 4 SPACE  &u_lt 5 TAB       &u_lt 8 RET  &u_lt 7 BSPC  &u_lt 9 DEL >; }; """

layer = """
    %(name)s { 
    display-name = "%(display)s"; 
    bindings = < 
        %(a1)s%(a2)s
        %(b1)s%(b2)s
        %(c1)s%(c2)s
        %(g1)s%(g2)s
        >; 
    }; 
"""


def getLayer(data, middle=100, cell=16):
    name, payload = re.match("(\w+)\s+.*<(.*)>.*", data).groups()

    rows = []
    i = 0
    row = ""
    for key in re.findall("&[^&>]*", payload):
        row += key.strip().ljust(cell)
        i += 1
        if i % (3 if i >= 37 else 6) == 0:
            rows.append(row.strip())
            row = ""

    
    
    return layer % dict(name=name.upper(), display=name.capitalize(), 
                        a1=rows[0].ljust(middle),
                        a2=rows[1],
                        b1=rows[2].ljust(middle),
                        b2=rows[3],
                        c1=rows[4].ljust(middle),
                        c2=rows[5],
                        g1=(" "*(3*cell) + rows[6]).ljust(middle),
                        g2=rows[7],                       
                        )



with open(sys.argv[1]) as f:
    for row in f.readlines():
        if "display-name" in row:
            print(getLayer(row))
        else:
            print(row, end="")
