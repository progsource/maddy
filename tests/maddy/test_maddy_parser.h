/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

#include <string>

const std::string testMarkdown =
  "# This is a test\n\
\n\
This should result in a praragraph\n\
it's that simple.\n\
\n\
* an *unordered* list\n\
  * with some **hierarchy**\n\
    1. and an _ordered_\n\
    * list\n\
    * directly\n\
  * inside\n\
\n\
```\n\
var c = 'blub';\n\
```\n\
\n\
> A Quote\n\
>\n\
> With some ~~text~~  blocks inside\n\
>\n\
> * even a list\n\
> * should be\n\
> * possible\n\
>\n\
\n\
And well `inline code` should also work.\n\
\n\
## Another Headline\n\
\n\
And not to forget [link to progsource](http://progsource.de) should work.\n\
And well - let's see how an image would be shown:\n\
\n\
![an image](http://progsource.de/img/progsource.png)\n\
\n\
---\n\
\n\
<a name=\"to top\"></a>\n\
\n\
### and more headlines\n\
\n\
- [ ] how\n\
- [ ] about\n\
  - [ ] a\n\
  - [x] nice\n\
- [x] check\n\
- [ ] list\n\
\n\
#### even a table\n\
\n\
|table>\n\
Left header|middle header|last header\n\
- | - | -\n\
cell 1|cell **2**|cell 3\n\
cell 4|cell 5|cell 6\n\
- | - | -\n\
foot a|foot b|foot c\n\
|<table\n\
\n\
##### h5\n\
###### h6\n\
\n\
";

const std::string testHtml =
  "<h1>This is a test</h1><p>This should result in a praragraph it's that "
  "simple. </p><ul><li>an <i>unordered</i> list<ul><li>with some "
  "<strong>hierarchy</strong><ol><li>and an "
  "<em>ordered</em></li><li>list</li><li>directly</li></ol></li><li>inside</"
  "li></ul></li></ul><pre><code>\nvar c = "
  "'blub';\n</code></pre><blockquote><p>A Quote  </p><p>With some <s>text</s>  "
  "blocks inside  </p><ul><li>even a list </li><li>should be </li><li>possible "
  "</li></ul></blockquote><p>And well <code>inline code</code> should also "
  "work. </p><h2>Another Headline</h2><p>And not to forget <a "
  "href=\"http://progsource.de\">link to progsource</a> should work. And well "
  "- let's see how an image would be shown: </p><p><img "
  "src=\"http://progsource.de/img/progsource.png\" alt=\"an image\"/> "
  "</p><hr/><p><a name=\"to top\"></a> </p><h3>and more headlines</h3><ul "
  "class=\"checklist\"><li><label><input type=\"checkbox\"/> "
  "how</label></li><li><label><input type=\"checkbox\"/> about<ul "
  "class=\"checklist\"><li><label><input type=\"checkbox\"/> "
  "a</label></li><li><label><input type=\"checkbox\" checked=\"checked\"/> "
  "nice</label></li></ul></label></li><li><label><input type=\"checkbox\" "
  "checked=\"checked\"/> check</label></li><li><label><input "
  "type=\"checkbox\"/> list</label></li></ul><h4>even a "
  "table</h4><table><thead><tr><th>Left header</th><th>middle "
  "header</th><th>last header</th></tr></thead><tbody><tr><td>cell "
  "1</td><td>cell <strong>2</strong></td><td>cell 3</td></tr><tr><td>cell "
  "4</td><td>cell 5</td><td>cell 6</td></tr></tbody><tfoot><tr><td>foot "
  "a</td><td>foot b</td><td>foot "
  "c</td></tr></tfoot></table><h5>h5</h5><h6>h6</h6>";
const std::string testHtml2 =
  "<h1>This is a test</h1><p>This should result in a praragraph it's that "
  "simple. </p><ul><li>an <i>unordered</i> list<ul><li>with some "
  "<strong>hierarchy</strong><ol><li>and an "
  "_ordered_</li><li>list</li><li>directly</li></ol></li><li>inside</li></ul></"
  "li></ul><pre><code>\nvar c = 'blub';\n</code></pre><blockquote><p>A Quote  "
  "</p><p>With some <s>text</s>  blocks inside  </p><ul><li>even a list "
  "</li><li>should be </li><li>possible </li></ul></blockquote><p>And well "
  "<code>inline code</code> should also work. </p><h2>Another "
  "Headline</h2><p>And not to forget <a href=\"http://progsource.de\">link to "
  "progsource</a> should work. And well - let's see how an image would be "
  "shown: </p><p><img src=\"http://progsource.de/img/progsource.png\" alt=\"an "
  "image\"/> </p><hr/><a name=\"to top\"></a><h3>and more headlines</h3><ul "
  "class=\"checklist\"><li><label><input type=\"checkbox\"/> "
  "how</label></li><li><label><input type=\"checkbox\"/> about<ul "
  "class=\"checklist\"><li><label><input type=\"checkbox\"/> "
  "a</label></li><li><label><input type=\"checkbox\" checked=\"checked\"/> "
  "nice</label></li></ul></label></li><li><label><input type=\"checkbox\" "
  "checked=\"checked\"/> check</label></li><li><label><input "
  "type=\"checkbox\"/> list</label></li></ul><h4>even a "
  "table</h4><table><thead><tr><th>Left header</th><th>middle "
  "header</th><th>last header</th></tr></thead><tbody><tr><td>cell "
  "1</td><td>cell <strong>2</strong></td><td>cell 3</td></tr><tr><td>cell "
  "4</td><td>cell 5</td><td>cell 6</td></tr></tbody><tfoot><tr><td>foot "
  "a</td><td>foot b</td><td>foot "
  "c</td></tr></tfoot></table><h5>h5</h5><h6>h6</h6>";
const std::string testHtml3 =
  "# This is a test <br/>This should result in a praragraph it's that simple. "
  "<br/>* an *unordered* list   * with some <strong>hierarchy</strong>     1. "
  "and an <em>ordered</em>     * list     * directly   * inside <br/>``` var c "
  "= 'blub'; ``` <br/>> A Quote > > With some ~~text~~  blocks inside > > * "
  "even a list > * should be > * possible > <br/>And well `inline code` should "
  "also work. <br/>## Another Headline <br/>And not to forget [link to "
  "progsource](http://progsource.de) should work. And well - let's see how an "
  "image would be shown: <br/>![an "
  "image](http://progsource.de/img/progsource.png) <br/>--- <br/><a name=\"to "
  "top\"></a> <br/>### and more headlines <br/>- [ ] how - [ ] about   - [ ] a "
  "  - [x] nice - [x] check - [ ] list <br/>#### even a table <br/>|table> "
  "Left header|middle header|last header - | - | - cell 1|cell "
  "<strong>2</strong>|cell 3 cell 4|cell 5|cell 6 - | - | - foot a|foot b|foot "
  "c |<table <br/>##### h5 ###### h6 <br/>";
