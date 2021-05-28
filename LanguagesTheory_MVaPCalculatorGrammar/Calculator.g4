grammar Calculator;

@header {
	import java.util.HashMap;
}

@members {
	public class Pair {
		private String key;
		private int value;

		public Pair(String key, int value) {
			this.key = key;
			this.value = value;
		}		

		public String getKey() {
			return this.key;
		}
	
		public int getValue() {
			return this.value;
		}
	}

	private HashMap<String, Pair> var_table = new HashMap<String, Pair>();
	private int index = 0;
	private int current_label = 0;
	
}

// regles de la grammaire
start returns [String code]

@init {
	$code = new String();
}

@after {
	System.out.println($code);
}
: (decl {
	$code += $decl.code;
})*
	NEWLINE*
	(instr {
	$code += $instr.code;
})*
	{
	$code += "HALT\n";
}
	EOF;


instrbloc returns [String code]

: '{'
	(instr {
	if ($code == null)
		$code = $instr.code;
	else
		$code += $instr.code;
})+	
	'}';


loopwhile returns [String code]

: 'while' '(' exb=boolexpr ')' endofinstr? inst=instr {
	int before = current_label;
	int after = current_label + 1;
	current_label += 2;
	$code = "LABEL " + String.valueOf(before) + "\n";
	$code += $exb.code;
	$code += "JUMPF " + String.valueOf(after) + "\n";
	$code += $inst.code;
	$code += "JUMP " + String.valueOf(before) + "\n";
	$code += "LABEL " + String.valueOf(after) + "\n";
}
| 'while' '(' exb=boolexpr ')' instb=instrbloc endofinstr {
	int before = current_label;
	int after = current_label + 1;
	current_label += 2;
	$code = "LABEL " + String.valueOf(before) + " \n";
	$code += $exb.code;
	$code += "JUMPF " + String.valueOf(after) + "\n";
	$code += $instb.code;
	$code += "JUMP " + String.valueOf(before) + "\n";
	$code += "LABEL " + String.valueOf(after) + "\n";
};


condelse returns [String code]

: test='else' endofinstr? instr {
	$code = $instr.code;
}
| 'else' instb=instrbloc endofinstr {
	$code = $instb.code;	
}
| test='else'? {
	$code = "";
};

condif returns [String code]

: 'if' '(' exb=boolexpr ')' endofinstr? inst=instr ce=condelse {
	int next = current_label;
	int next_bis = current_label + 1;
	current_label += 2;
	$code = $exb.code;
	$code += "JUMPF " + String.valueOf(next) + "\n";
	$code += $inst.code;
	$code += "JUMP " + String.valueOf(next_bis) + "\n";
	$code += "LABEL " + String.valueOf(next) + "\n";
	$code += $ce.code;
	$code += "LABEL " + String.valueOf(next_bis) + "\n";
}
| 'if' '(' exb=boolexpr ')' instb=instrbloc endofinstr ce=condelse {
	int next = current_label;
	int next_bis = current_label + 1;
	current_label += 2;
	$code = $exb.code;
	$code += "JUMPF " + String.valueOf(next) + "\n";
	$code += $instb.code;
	$code += "JUMP " + String.valueOf(next_bis) + "\n";
	$code += "LABEL " + String.valueOf(next) + "\n";
	$code += $ce.code;
	$code += "LABEL " + String.valueOf(next_bis) + "\n";
};


decl returns [String code]

: t=TYPE v=VARIABLE endofinstr {
	if ($t.text.equals("int") || $t.text.equals("bool"))
		$code = "PUSHI 0\n";
	var_table.put($v.text, new Pair($t.text, index));
	index++;
};


instr returns [String code]

: exp=expr endofinstr {
	$code = $exp.code;	
}
| bexp=boolexpr endofinstr {
	$code = $bexp.code;
}
| ass=assign endofinstr {
	$code = $ass.code;
}
| iof=io endofinstr {
	$code = $iof.code;
}
| lw=loopwhile {
	$code = $lw.code;
}
| ci=condif {
	$code = $ci.code;
}
| endofinstr {
	$code = "";
};


assign returns [String code]

: v=VARIABLE '=' ex=expr { 
	Pair info = var_table.get($v.text);
	String type_tmp = info.getKey();
	int id_tmp = info.getValue();
	
	if (type_tmp.equals("bool"))
		throw new IllegalArgumentException("Error : implicit conversion from 'bool' to 'int'");
	$code = $ex.code + "STOREG " + String.valueOf(id_tmp) + " \n";
}

| v=VARIABLE '=' exb=boolexpr { 
	Pair info = var_table.get($v.text);
	String type_tmp = info.getKey();
	int id_tmp = info.getValue();
	
	if (!type_tmp.equals("bool"))
		throw new IllegalArgumentException("Error : implicit conversion from 'int' to 'bool'");
	$code = $exb.code + "STOREG " + String.valueOf(id_tmp) + " \n";
};


expr returns [String code]

: ope=('+'|'-') a=expr {
	$code =  $a.code;
	if ($ope.text.equals("-")) {
		$code += "PUSHI -1 \n";
		$code += "MUL \n";
	}
}
| '(' a=expr ')' {
	$code = $a.code;
}
| a=expr ope=('*'|'/') b=expr {
	if ($ope.text.equals("*")) {
		$code = $a.code + $b.code + "MUL" + " \n";
	} else {
		$code = $a.code + $b.code + "DIV" + " \n";
	}
}
| a=expr ope=('+'|'-') b=expr {
	if ($ope.text.equals("+")) {
		$code = $a.code + $b.code + "ADD" + " \n";
	} else {
		$code = $a.code + $b.code + "SUB" + " \n";
	}
}
| f=ENTIER {
	$code = "PUSHI " + $f.text + " \n";
}
| v=VARIABLE {
	Pair info = var_table.get($v.text);
	String type_tmp = info.getKey();
	int id_tmp = info.getValue();
	
	if (type_tmp.equals("bool"))
		throw new IllegalArgumentException("Error : implicit conversion from 'bool' to 'int'");
	$code = "PUSHG " + String.valueOf(id_tmp) + " \n";
};


boolexpr returns [String code]

: 'not' a=boolexpr {
	$code = $a.code + "PUSHI 0" + " \n";
	$code += "EQUAL" + " \n";	
}
| '(' a=boolexpr ')' {
	$code = $a.code;
}
| a=boolexpr 'and' b=boolexpr {
	$code = $a.code + $b.code + "PUSHI 1 \n";
	$code += "EQUAL \n";
	$code += "EQUAL \n";
}
| a=boolexpr 'or' b=boolexpr {
	$code = $a.code + $b.code + "PUSHI 0 \n";
	$code += "EQUAL \n";
	$code += "SUPEQ \n";
}
| c=expr comp=COMP d=expr {
	$code = $c.code + $d.code;
	if ($comp.text.equals("==")) {
		$code += "EQUAL" + " \n";
	} else if ($comp.text.equals("<>") || $comp.text.equals("!=")) {
		$code += "NEQ" + " \n";
	} else if ($comp.text.equals("<")) {
		$code += "INF" + " \n";
	} else if ($comp.text.equals(">")) {
		$code += "SUP" + " \n";
	} else if ($comp.text.equals(">=")) {
		$code += "SUPEQ" + " \n";
	} else {
		$code += "INFEQ" + " \n";
	}
}
| f=BOOL {
	boolean val = Boolean.parseBoolean($f.text);
	if (val == true) {
		$code = "PUSHI 1" + " \n";
	} else {
		$code = "PUSHI 0" + " \n";
	}
}
| v=VARIABLE {
	Pair info = var_table.get($v.text);
	String type_tmp = info.getKey();
	int id_tmp = info.getValue();
	
	if (!type_tmp.equals("bool"))
		throw new IllegalArgumentException("Error : implicit conversion from 'int' to 'bool'");
	$code = "PUSHG " + String.valueOf(id_tmp) + " \n";
};


io returns [String code]

: READ '(' v=VARIABLE ')' {
	Pair info = var_table.get($v.text);
	String type_tmp = info.getKey();
	int id_tmp = info.getValue();
	
	$code = "READ\n";
	$code += "STOREG " + String.valueOf(id_tmp) + " \n";
}
| WRITE '(' exp=expr ')' {
	$code = $exp.code + "WRITE\nPOP\n";
}
| WRITE '(' expb=boolexpr ')' {
	$code = $expb.code + "WRITE\nPOP\n";
};


endofinstr 

: (NEWLINE | ';')+;


// regles du lexer. Skip pour dire ne rien faire
NEWLINE : '\r'? '\n';
WS : (' '|'\t')+ -> skip;
ENTIER : ('0'..'9')+;
BOOL : ('false'|'true');
COMP : ('=='|'<>'|'<'|'>'|'<='|'>='|'!=');
TYPE : 'int' | 'bool';
WRITE : 'writeln'|'println';
READ : 'readln';
VARIABLE : ('a'..'z'|'A'..'Z')+ ('0'..'9'|'_')*;
UNMATCH : . -> skip;

