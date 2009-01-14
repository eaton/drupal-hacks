<?php
function loltemplate_parse($code) {
  $search = "/(HAI(?:.*?)(?:KTHXBYE|KTHXBAI))/se";
  $replace = '_loltemplate_convert_chunk("$1")';
  $code = preg_replace($search, $replace, $code);
  return $code;
}

function _loltemplate_convert_chunk($code) {
  $code = $code;
  $replacements = array(
    // Collapse whitespace
    '/^\s+/' => '',
    '/\s+/' => ' ',
    '/\n+/' => "\n",
    '/,/' => "\n",
    '/[\!\?1]{3,}/' => "\n",
    // Basic PHP delimiters
    '/HAI/' => '<?php ',
    '/KTHXBYE|KTHXBAI/' => ' ?>',
    // Single and multiline comments FTW.
    '/BTW (.*)$/' => "// $1",
    '/OBTW (.*?)TLDR/s/' => '\/\*\1\*\/',
    // These don't include line matching because we should be able to
    // use them inline in other expressions.
    // Math goes here
    "/SUM OF ([a-zA-Z0-9_-]+) ([a-zA-Z0-9_-]+)/" => "\1 + \2",
    "/DIFF OF ([a-zA-Z0-9_-]+) ([a-zA-Z0-9_-]+)/" => "\1 - \2",
    "/PRODUKT OF ([a-zA-Z0-9_-]+) ([a-zA-Z0-9_-]+)/" => "\1 * \2",
    "/QUOSHUNT OF ([a-zA-Z0-9_-]+) ([a-zA-Z0-9_-]+)/" => "\1 / \2",
    "/MOD OF ([a-zA-Z0-9_-]+) ([a-zA-Z0-9_-]+)/" => "\1 ^ \2",
    "/BIGGR OF ([a-zA-Z0-9_-]+) ([a-zA-Z0-9_-]+)/" => "max(\1, \2)",
    "/SMALLR OF ([a-zA-Z0-9_-]+) ([a-zA-Z0-9_-]+)/" => "min(\1, \2)",
    // Boolean comparisons go here
    "/BOTH OF (.+) AN (.+)/" => "(\1 && \2) == TRUE",
    "/EITHER OF (.+) AN (.+)/" => "(\1 || \2) == TRUE",
    "/WON OF (.+) AN (.+)/" => "(\1 xor \2) == TRUE",
    "/BOTH SAEM (.+) AN (.+)/" => "\1 == \2",
    "/DIFFRINT (.+) AN (.+)/" => "\1 != \2",
    "/BOTH RILLY SAEM (.+) AN (.+)/" => "\1 === \2",
    "/RILLY DIFFRINT (.+) AN (.+)/" => "\1 !== \2",
    // Variable declarations!
    // Array are still kinda janky. Nested declarations don't work.
    '/(?:BUCKIT|BUCKET)(.*)LID/s' => 'array(\1)',
    '/(?:BUCKIT|BUCKET)/' => 'array()',
    '/^I HAS|HAZ A ([a-zA-Z0-9_-]+) ITZ /' => '\1 = ',
    '/^I HAS|HAZ A ([a-zA-Z0-9_-]+)$/' => '\1 = NULL',
    '/([a-zA-Z0-9_-]+) R (.+)$/' => '\1 = \2',
    '/(.+) IZ (.+)/' => '\1 => \2,',
    // Basic php commands and operators like concatenation, printing,
    // incrementing, and decrementing.
    '/ SMOOSH /' => ' . ',
    '/VISIBLE /' => 'echo ',
    '/CAN HAS|HAZ (.*)$/' => 'include_once(\1)',
    '/MUST HAS|HAZ (.*)$/' => 'require_once(\1)',
    '/([a-zA-Z0-9_-]+) UPUP\!/' => '\$\1++',
    '/([a-zA-Z0-9_-]+) DOWNDOWN\!/' => '\$\1--',
    // Flow control
    "/^(.*)$[\s]*O RLY\?$[\s]*YA RLY/" => "if (\1) {",
    "/^MEBBE (.*?)$/" => "} elseif (\1) {",
    "/^NO WAI/" => "} else {",
    "/^(.*)$[\s]*WTF\?$/" => "switch (\1) {",
    "/^OMG (.*?)$/" => "case \1:",
    "/^OMGWTF$/" => "default:",
    '/^IM IN YR (.+) ITZA (.+)$/' => 'foreach (\1 as \2) {',
    '/^IM IN YR (.+) ITZA (.+?) FULLA (.+?)$/' => 'foreach (\1 as \2 => \3) {',
    '/^OIC$/' => '}',
    // Function declaration
   "/^HOW DUZ I ([a-zA-Z0-9_-]+)\s*(?:YR )(.*)/" => "function \1(\2) {",
    " AN YR " => ", ",
    "/^FOUND YR (.+)$/" => "return \1",
    "/^GTFO$/" => "return",
    "/^IF U SAY SO$/" => "}",
    '/^SO IM LIKE ([a-zA-Z0-9_-]+) WITH (.*)$/' => '\1(\2)',
    // Escaped characters
    '/ AN /' => '',
    '/(?<!\:)\:\/)' => "\n",
    '/(?<!\:)\:>/' => "  ",
    '/(?<!\:)\:o/' => "\g",
    '/(?<!\:)\:"/' => "\"",
    '/\:\:/' => ":",
    // Stick semicolons on the end of non-empty lines that don't end with
    // ; or : or } or , or /. This will catch commented lines actually. Need to
    // solve that later.
    '/^(.*?[^\:|\;|\}|,|\/])$/' => '\1;',
  );
  $code = preg_replace(array_keys($replacements), array_values($replacements), $code);
  return $code;
}
