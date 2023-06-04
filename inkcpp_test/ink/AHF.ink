VAR TEST_VAR = -99
VAR test_tweets = 4
VAR test_banters = 1

=== test_knot1 ===
#reset_on_break
line 1 #loc:loc-line-1 #in #op:1 #cl:1
line 2 #loc:loc-line-2 #en #op:1 #cl:1
line 3 #loc:loc-line-3 #in #op:1 #cl:1
->DONE

=== test_tweet1 ===
test tweet 1 #op:0.5 #cl:0.5
->DONE

=== test_tweet2 ===
test tweet 2 #op:0.5 #cl:0.5
->DONE

=== test_tweet3 ===
test tweet 3 #op:0.5 #cl:0.5
->DONE

=== test_tweet4_tired ===
test tweet4 tired #op:0.5 #cl:0.5
->DONE

=== test_banter1 ===
a #in #op:0.5 #cl:0.5
b #en #op:0.5 #cl:0.5
c #in #op:0.5 #cl:0.5
->DONE

->test_knot
===test_knot
line 110#loc:loc-line-1#in#op:1#cl:1
line 2#loc:loc-line-2#en#op:1#cl:1
line 3#loc:loc-line-3#in#op:1#cl:1
->DONE

