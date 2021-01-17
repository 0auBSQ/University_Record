package roman;

import java.util.HashMap;
import java.util.Map;

import org.junit.*;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.*;


public class RomanNumberTest {
  public static final HashMap<Integer, String> KNOWN_VALUES = new HashMap<>();
  static {
    KNOWN_VALUES.put(1, "I");
    KNOWN_VALUES.put(2, "II");
    KNOWN_VALUES.put(3, "III");
    KNOWN_VALUES.put(4, "IV");
    KNOWN_VALUES.put(5, "V");
    KNOWN_VALUES.put(6, "VI");
    KNOWN_VALUES.put(7, "VII");
    KNOWN_VALUES.put(8, "VIII");
    KNOWN_VALUES.put(9, "IX");
    KNOWN_VALUES.put(10, "X");
    KNOWN_VALUES.put(50, "L");
    KNOWN_VALUES.put(100, "C");
    KNOWN_VALUES.put(500, "D");
    KNOWN_VALUES.put(1000, "M");
    KNOWN_VALUES.put(31, "XXXI");
    KNOWN_VALUES.put(148, "CXLVIII");
    KNOWN_VALUES.put(294, "CCXCIV");
    KNOWN_VALUES.put(312, "CCCXII");
    KNOWN_VALUES.put(421, "CDXXI");
    KNOWN_VALUES.put(528, "DXXVIII");
    KNOWN_VALUES.put(621, "DCXXI");
    KNOWN_VALUES.put(782, "DCCLXXXII");
    KNOWN_VALUES.put(870, "DCCCLXX");
    KNOWN_VALUES.put(941, "CMXLI");
    KNOWN_VALUES.put(1043, "MXLIII");
    KNOWN_VALUES.put(1110, "MCX");
    KNOWN_VALUES.put(1226, "MCCXXVI");
    KNOWN_VALUES.put(1301, "MCCCI");
    KNOWN_VALUES.put(1485, "MCDLXXXV");
    KNOWN_VALUES.put(1509, "MDIX");
    KNOWN_VALUES.put(1607, "MDCVII");
    KNOWN_VALUES.put(1754, "MDCCLIV");
    KNOWN_VALUES.put(1832, "MDCCCXXXII");
    KNOWN_VALUES.put(1993, "MCMXCIII");
    KNOWN_VALUES.put(2074, "MMLXXIV");
    KNOWN_VALUES.put(2152, "MMCLII");
    KNOWN_VALUES.put(2212, "MMCCXII");
    KNOWN_VALUES.put(2343, "MMCCCXLIII");
    KNOWN_VALUES.put(2499, "MMCDXCIX");
    KNOWN_VALUES.put(2574, "MMDLXXIV");
    KNOWN_VALUES.put(2646, "MMDCXLVI");
    KNOWN_VALUES.put(2723, "MMDCCXXIII");
    KNOWN_VALUES.put(2892, "MMDCCCXCII");
    KNOWN_VALUES.put(2975, "MMCMLXXV");
    KNOWN_VALUES.put(3051, "MMMLI");
    KNOWN_VALUES.put(3185, "MMMCLXXXV");
    KNOWN_VALUES.put(3250, "MMMCCL");
    KNOWN_VALUES.put(3313, "MMMCCCXIII");
    KNOWN_VALUES.put(3408, "MMMCDVIII");
    KNOWN_VALUES.put(3501, "MMMDI");
    KNOWN_VALUES.put(3610, "MMMDCX");
    KNOWN_VALUES.put(3743, "MMMDCCXLIII");
    KNOWN_VALUES.put(3844, "MMMDCCCXLIV");
    KNOWN_VALUES.put(3888, "MMMDCCCLXXXVIII");
    KNOWN_VALUES.put(3940, "MMMCMXL");
    KNOWN_VALUES.put(3999, "MMMCMXCIX");
  }

  // Tests de reussite

  // String -> RomanNumber
  @Test
  public void fromRomanKnownValues() {
    for (Map.Entry<Integer, String> v : KNOWN_VALUES.entrySet()) {
      assertThat(RomanNumber.valueOf(v.getValue()).intValue(), is(v.getKey()));
    }
  }

  // int -> RomanNumber
  @Test
  public void toRomanKnownValues() {
    for (Map.Entry<Integer, String> v : KNOWN_VALUES.entrySet()) {
      assertThat(RomanNumber.valueOf(v.getKey()).toString(), is(v.getValue()));
    }
  }

  // Tests d'echec

  // double -> RomanNumber
  @Test(expected = IllegalArgumentException.class)
  public void toRomanNonInt() {
    RomanNumber.valueOf(14.91);
  }

  // int -> RomanNumber
  @Test(expected = IllegalArgumentException.class)
  public void toRomanNegative() {
    RomanNumber.valueOf(-1);
  }

  // String -> RomanNumber
  @Test(expected = IllegalArgumentException.class)
  public void toRomanInvalid() {
    RomanNumber.valueOf("XCX");
  }

  // String -> RomanNumber
  @Test(expected = IllegalArgumentException.class)
  public void toRomanRepetitions() {
    RomanNumber.valueOf("XVIIII");
  }

  // int -> RomanNumber
  @Test(expected = IllegalArgumentException.class)
  public void toRomanOverflow() {
    RomanNumber.valueOf(4000);
  }

  // int -> RomanNumber
  @Test(expected = IllegalArgumentException.class)
  public void toRomanZero() {
    RomanNumber.valueOf(0);
  }

  // String -> RomanNumber
  @Test(expected = IllegalArgumentException.class)
  public void toRomanDoubling() {
    RomanNumber.valueOf("VV");
  }

  // Validation tests

  // int -> RomanNumber
  @Test
  public void validateRoman() {
    for (int i = 1; i <= 3999; i++) {
      assertThat(RomanNumber.valueOf(RomanNumber.valueOf(i).toString()).intValue(), is(i));
    }
  }

  // int -> RomanNumber
  @Test
  public void capitalizeRoman() {
    for (int i = 1; i <= 3999; i++) {
      assertThat(RomanNumber.valueOf(i).toString(), is(RomanNumber.valueOf(i).toString().toUpperCase()));
    }
  }
  
  // Comparable tests
  @Test
  public void testComparable() {
	  assertThat(RomanNumber.valueOf("XVI").compareTo(RomanNumber.valueOf("XX")), is(-4));
	  assertThat(RomanNumber.valueOf("XVI").compareTo(20), is(-4));
	  assertThat(RomanNumber.valueOf("XVI").compareTo(16), is(0));
	  assertThat(RomanNumber.valueOf("XVI").compareTo((double)15), is(1));
  }

}
