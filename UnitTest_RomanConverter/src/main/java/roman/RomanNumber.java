
package roman;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

public final class RomanNumber extends Number implements Comparable<RomanNumber> {

  public static final long serialVersionUID = 1L;

  // Table des symboles
  private static final LinkedHashMap<String, Integer> SYMBOLS = new LinkedHashMap<>();
  static {
    SYMBOLS.put("M",  1000);
    SYMBOLS.put("CM", 900);
    SYMBOLS.put("D",  500);
    SYMBOLS.put("CD", 400);
    SYMBOLS.put("C",  100);
    SYMBOLS.put("XC", 90);
    SYMBOLS.put("L",  50);
    SYMBOLS.put("XL", 40);
    SYMBOLS.put("X",  10);
    SYMBOLS.put("IX", 9);
    SYMBOLS.put("V",  5);
    SYMBOLS.put("IV", 4);
    SYMBOLS.put("I", 1);
  }

  // Expression reguliere de validation
  private static final Pattern VALIDATION_RE = Pattern.compile(
      "^M{0,3}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$");

  private final int value;

  public RomanNumber(String romanValue) {
    this.value = fromRoman(romanValue);
  }

  public RomanNumber(int value) {
	if (value < 1 || value > 3999) {
		throw new IllegalArgumentException(value + " : Out of range [1-3999]");
	}
    this.value = value;
  }

  public RomanNumber(double value) {
	if (Math.floor(value) != value) {
		throw new IllegalArgumentException(value + " : Isn't an Integer");
	}
	if (Math.floor(value) < 1 || Math.floor(value) > 3999) {
		throw new IllegalArgumentException(value + " : Out of range [1-3999]");
	}
	this.value = (int)value;
  }

  /**
   * @{inheritDoc}
   */
  @Override
  public double doubleValue() {
	return (double)this.value;
  }

  /**
   * @{inheritDoc}
   */
  @Override
  public float floatValue() {
    return (float)this.value;
  }

  /**
   * @{inheritDoc}
   */
  @Override
  public int intValue() {
    return this.value;
  }

  /**
   * @{inheritDoc}
   */
  @Override
  public long longValue() {
    return (long)this.value;
  }

  @Override
  public String toString() {
    return toRoman(this.value);
  }

  public static RomanNumber valueOf(String roman) {
    return new RomanNumber(roman);
  }

  public static RomanNumber valueOf(int value) {
    return new RomanNumber(value);
  }

  public static RomanNumber valueOf(double value) {
    return new RomanNumber(value);
  }
  
  public int compareTo(RomanNumber o) {
	 return (this.value - o.intValue());
  }
  
  public int compareTo(int o) {
	 return (this.value - o);
  }
  
  public int compareTo(float o) {
	 return (this.value - (int)o);
  }
  
  public int compareTo(double o) {
	 return (this.value - (int)o);
  }
  
  public int compareTo(long o) {
	 return (this.value - (int)o);
  }

  private static int fromRoman(String romanValue) {
    int resultat = 0;
	int index = 0;

	if (!(VALIDATION_RE.matcher(romanValue)).matches())  {
		throw new IllegalArgumentException(romanValue + " : Invalid roman string");
	}
	for (Map.Entry<String, Integer> entry : SYMBOLS.entrySet()) {
		int offset = (entry.getKey()).length();
		while ((index + offset - 1) < romanValue.length() && romanValue.substring(index, index + offset).equals(entry.getKey())) {
			resultat += entry.getValue();
			index += offset;
		}
	}
    return resultat;
  }

  private static String toRoman(int value) {
    if (value < 1 || value >= 4000) {
		throw new IllegalArgumentException(value + " : Out of range [1-3999]");
	}
	String roman = "";
	for (Map.Entry<String, Integer> entry : SYMBOLS.entrySet()) {
		while (value >= entry.getValue()) {
			roman += entry.getKey();
			value -= entry.getValue();
		}
	}
    return roman;
  }
}
