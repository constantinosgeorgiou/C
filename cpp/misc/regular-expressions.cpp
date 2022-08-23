#include <cstdio>
#include <iostream>
#include <regex>
#include <string>

using namespace std;

void match_regex(void) {
  string text = "subject";

  regex rexpr("(sub)(.*)");
  // Indicates if there is a match between the regular expression e and a
  // C-style null-terminated string.
  bool match = regex_match(text.c_str(), rexpr);
  if (match) {
    printf("String literal matched\n");
  }
  text = "whatever";
  match = regex_match(text.c_str(), rexpr);
  if (!match) {
    printf("String literal NOT matched\n");
  }
  text = "subject";
  cmatch cm;
  match = regex_match(text.c_str(), cm, rexpr);
  printf("match: %d, cm.size(): %ld\n", match, cm.size());

  smatch sm;
  match = regex_match(text, sm, rexpr);
  printf("match: %d, sm.size(): %ld\n", match, sm.size());

  match = regex_match(text.cbegin(), text.cend(), sm, rexpr);
  printf("match: %d, sm.size(): %ld\n", match, sm.size());

  match = regex_match(text.c_str(), cm, rexpr, regex_constants::match_default);
  printf("match: %d, sm.size(): %ld\n", match, sm.size());
  for (unsigned int i = 0; i < sm.size(); i++) {
    printf("[%s/%s] ", sm[i].first.base(), sm[i].str().c_str());
  }
  printf("\n");
}

void search_regex() {
  string text = "this subject has a submarine as a subsequence";
  smatch match;
  regex re("\\b(sub)([^ ]*)");

  printf("Target sequence: %s\n", text.c_str());
  printf("Regular expression: /\\b(sub)([^ ]*)/\n");
  printf("Matches and submatches found: \n");
  while (regex_search(text, match, re)) {
    for (auto x : match) {
      printf(" - '%s' | '%s' | '%s'\n", x.first.base(), x.second.base(),
             x.str().c_str());
    }
    printf("\n");
    text = match.suffix().str();
  }
}

void replace_regex(void) {
  string text = "there is a subsequence in the string\n";
  regex re("\\b(sub)([^ ]*)");

  string replaced = regex_replace(text, re, "sub-$2");
  printf("Replaced: '%s'\n", replaced.c_str());

  string result;
  regex_replace(back_inserter(result), text.begin(), text.end(), re, "$2");
  printf("replaced: '%s', result: '%s'\n", replaced.c_str(), result.c_str());

  result =
      regex_replace(text, re, "$1 and $2", regex_constants::format_no_copy);
  printf("Result: '%s'\n", result.c_str());
}

void iterate_regex(void) {
  string text = "seize the means of production";
  regex words_regex("[^\\s]+");
  auto words_begin = sregex_iterator(text.begin(), text.end(), words_regex);
  auto words_end = sregex_iterator();
  printf("Text: '%s'\n", text.c_str());
  printf("Found %ld words:\n", distance(words_begin, words_end));
  for (sregex_iterator i = words_begin; i != words_end; i++) {
    smatch match = *i;
    printf(" - %s\n", match.str().c_str());
  }
}

void token_regex() {
  string text = "No gods no masters\n";
  regex ws_re("\\s+");
  printf("Text: '%s'\n", text.c_str());
  copy(sregex_token_iterator(text.begin(), text.end(), ws_re, -1),
       sregex_token_iterator(), ostream_iterator<string>(cout, "\n"));

  string html =
      "<p><a href=\"http://tutorialspoint.com\">google</a> "
      "< a HREF =\"http://indiabbc.com\">cppreference</a>\n</p>";

  regex url_re("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"", regex::icase);
  printf("html: '%s'\n", html.c_str());
  copy(sregex_token_iterator(html.begin(), html.end(), url_re, 1),
       sregex_token_iterator(), ostream_iterator<string>(cout, "\n"));
}

int main(int argc, char const *argv[]) {
  // match_regex();
  // search_regex();
  // replace_regex();
  // iterate_regex();
  token_regex();
  return 0;
}
