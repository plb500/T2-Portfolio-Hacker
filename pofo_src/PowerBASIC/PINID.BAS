DEF FNGetClockSeconds#
  REG 1, 0
  CALL INTERRUPT 26
  timeHigh# = REG(3)
  timeLow# = REG(4)
  FNGetClockSeconds# = ((timeHigh# * 65536) + timeLow#) / 18.2
END DEF

DEF FNGetParInput!
  FNGetParInput! = INP(32888)
END DEF

DEF FNPutParOutput(outData%)
  IF outData > 31 THEN
    EXIT DEF
  END IF

  lowBits% = outData% AND 3
  highBits% = (outData% AND 28) * 2
  allBits% = lowBits% OR highBits%
  OUT 32890, allBits%
  FNPutParOutput = allBits%
END DEF

DEF FNIntToStr$(intVal%)
  IF intVal% = 0 THEN
    FNIntToStr$ = "0"
    EXIT DEF
  ELSEIF intVal% = 1 THEN
    FNIntToStr$ = "1"
    EXIT DEF
  ELSEIF intVal% = 2 THEN
    FNIntToStr$ = "2"
    EXIT DEF
  ELSEIF intVal% = 3 THEN
    FNIntToStr$ = "3"
    EXIT DEF
  ELSEIF intVal% = 4 THEN
    FNIntToStr$ = "4"
    EXIT DEF
  ELSEIF intVal% = 5 THEN
    FNIntToStr$ = "5"
    EXIT DEF
  ELSEIF intVal% = 6 THEN
    FNIntToStr$ = "6"
    EXIT DEF
  ELSEIF intVal% = 7 THEN
    FNIntToStr$ = "7"
    EXIT DEF
  ELSEIF intVal% = 8 THEN
    FNIntToStr$ = "8"
    EXIT DEF
  ELSEIF intVal% = 9 THEN
    FNIntToStr$ = "9"
    EXIT DEF
  ELSE
    FNIntToStr$ = "X"
    EXIT DEF
  END IF
  FNIntToStr$ = "X"
END DEF


OUT 32891, 144
wroteOut% = FNPutParOutput(31)

scrollLength% = 40
DIM scrollLines$(2)
scrollLines$(0) = "1234567890123456789012345678901234567890"
scrollLines$(1) = "5345687267879876435879234583764578347586"
scrollLines$(2) = "4203287443651840826725039825960947324357"

currentTime# = FNGetClockSeconds#
validPin% = 0
pinDone% = 0
state% = 0
portAData! = 0
stateTimeout# = 0
stateDelay# = 0.25

DIM pinDigits$(4)
pinDigits$(0) = ""
pinDigits$(1) = ""
pinDigits$(2) = ""
pinDigits$(3) = ""


PRINT "PPPPP   IIIIIII   N    N"
PRINT "P   PP     I      NN   N IDENTIFICATION"
PRINT "P   PP     I      N N  N"
PRINT "PPPPP      I      N  N N   PROGRAM"
PRINT "P          I      N   NN"
PRINT "P       IIIIIII   N    N"
PRINT ""
PRINT "Strike a key when ready...";

WHILE NOT INSTAT
WEND
userInput$ = INKEY$

PRINT ""

' Loop until PIN received
WHILE pinDone% = 0
  FOR x = 0 TO 2
    PRINT scrollLines$(x);
  NEXT x

  currentTime# = FNGetClockSeconds#
  IF state% = 0 THEN
    portAData! = FNGetParInput!
    IF portAData! >= 128 THEN
      state% = 1
      stateTimeout# = stateTimeout# + stateDelay#
      wroteOut% = FNPutParOutput%(4)
      wroteOut% = FNPutParOutput%(0)
    END IF
  ELSEIF state% = 1 THEN
    IF currentTime# > stateTimeout# THEN
      portAData! = FNGetParInput!
      pin! = portAData! AND 15
      pinDigits$(0) = FNIntToStr(pin!)
      state% = 2
      stateTimeout# = stateTimeout# + stateDelay#
      wroteOut% = FNPutParOutput%(4)
    END IF
  ELSEIF state% = 2 THEN
    IF currentTime# > stateTimeout# THEN
      state% = 3
      stateTimeout# = stateTimeout# + stateDelay#
      wroteOut% = FNPutParOutput%(4)
      wroteOut% = FNPutParOutput%(5)
      wroteOut% = FNPutParOutput%(1)
    END IF
  ELSEIF state% = 3 THEN
    IF currentTime# > stateTimeout# THEN
      portAData! = FNGetParInput!
      pin! = portAData! AND 15
      pinDigits$(1) = FNIntToStr(pin!)
      state% = 4
      stateTimeout# = stateTimeout# + stateDelay#
      wroteOut% = FNPutParOutput%(4)
    END IF
  ELSEIF state% = 4 THEN
    IF currentTime# > stateTimeout# THEN
      state% = 5
      stateTimeout# = stateTimeout# + stateDelay#
      wroteOut% = FNPutParOutput%(4)
      wroteOut% = FNPutParOutput%(6)
      wroteOut% = FNPutParOutput%(2)
    END IF
  ELSEIF state% = 5 THEN
    IF currentTime# > stateTimeout# THEN
      portAData! = FNGetParInput!
      pin! = portAData! AND 15
      pinDigits$(2) = FNIntToStr(pin!)
      state% = 6
      stateTimeout# = stateTimeout# + stateDelay#
      wroteOut% = FNPutParOutput%(4)
    END IF
  ELSEIF state% = 6 THEN
    IF currentTime# > stateTimeout# THEN
      state% = 7
      stateTimeout# = stateTimeout# + stateDelay#
      wroteOut% = FNPutParOutput%(4)
      wroteOut% = FNPutParOutput%(7)
      wroteOut% = FNPutParOutput%(3)
    END IF
  ELSEIF state% = 7 THEN
    IF currentTime# > stateTimeout# THEN
      portAData! = FNGetParInput!
      pin! = portAData! AND 15
      pinDigits$(3) = FNIntToStr(pin!)
      pinDone% = 1
      wroteOut% = FNPutParOutput%(4)
    END IF
  END IF
WEND


' Decrement display to final PIN length
WHILE scrollLength% > 4
  FOR x = 0 TO 2
    FOR y = 0 to 2
      IF scrollLength% = 40 THEN
        PRINT scrollLines$(y);
      ELSE
        PRINT scrollLines$(y)
      END IF
    NEXT y
  NEXT x

  ' Reduce scroll line length
  scrollLength% = scrollLength% - 2
  FOR x = 0 TO 2
    scrollLines$(x) = LEFT$(scrollLines$(x), scrollLength%)
  NEXT x
WEND

' Scroll and insert PIN digits
FOR x = 1 TO 4
  FOR y = 0 TO 2
    MID$(scrollLines$(y), x) = pinDigits$(x - 1)
  NEXT y

  FOR z = 0 TO 3
    FOR y = 0 TO 2
      PRINT scrollLines$(y)
    NEXT y
  NEXT z
NEXT x

' Print final display
PRINT ""
PRINT "PIN IDENTIFICATION NUMBER: ";
FOR x = 0 TO 3
  PRINT pinDigits$(x);
NEXT x
PRINT ""

END
