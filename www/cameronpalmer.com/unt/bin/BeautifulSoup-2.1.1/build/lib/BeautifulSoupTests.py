"""Unit tests for Beautiful Soup.

These tests make sure the Beautiful Soup works as it should. If you
find a bug in Beautiful Soup, the best way to express it is as a test
case like this that fails."""

import unittest
from BeautifulSoup import *

class SoupTest(unittest.TestCase):

    def assertSoupEquals(self, toParse, rep=None, c=BeautifulSoup):
        """Parse the given text and make sure its string rep is the other
        given text."""
        if rep == None:
            rep = toParse
        self.assertEqual(str(c(toParse)), rep)

class FollowThatTag(SoupTest):

    "Tests the various ways of fetching tags from a soup."

    def setUp(self):        
        ml = """
        <a id="x">1</a>
        <a id="a">2</a>
        <b id="b">3</a>
        <b id="x">4</a>
        <ac width=100>4</ac>"""
        self.soup = BeautifulStoneSoup(ml)

    def testFetchByName(self):
        matching = self.soup('a')
        self.assertEqual(len(matching), 2)
        self.assertEqual(matching[0].name, 'a')
        self.assertEqual(matching, self.soup.fetch('a'))

    def testFetchByAttribute(self):
        matching = self.soup.fetch(attrs={'id' : 'x'})
        self.assertEqual(len(matching), 2)
        self.assertEqual(matching[0].name, 'a')
        self.assertEqual(matching[1].name, 'b')

        self.assertEqual(len(self.soup.fetch(attrs={'id' : None})), 1)

        self.assertEqual(len(self.soup.fetch(attrs={'width' : 100})), 1)

    def testFetchByList(self):
        matching = self.soup(['a', 'ac'])
        self.assertEqual(len(matching), 3)

    def testFetchByHash(self):
        matching = self.soup({'a' : True, 'b' : True})
        self.assertEqual(len(matching), 4)

    def testFetchText(self):
        soup = BeautifulSoup(u"<html>\xbb</html>")
        self.assertEqual(soup.fetchText(re.compile('.*')), [u'\xbb'])    

    def testFetchByRE(self):
        import re
        r = re.compile('a.*')
        self.assertEqual(len(self.soup(r)), 3)
        
    def testFetchByMethod(self):
        def matchTagWhereIDMatchesName(tag):
            return tag.name == tag.get('id')

        matching = self.soup.fetch(matchTagWhereIDMatchesName)
        self.assertEqual(len(matching), 2)
        self.assertEqual(matching[0].name, 'a')

    def testParents(self):
        soup = BeautifulSoup('<ul id="foo"></ul><ul id="foo"><ul><ul id="foo" a="b"><b>Blah')
        b = soup.b
        self.assertEquals(len(b.fetchParents('ul', {'id' : 'foo'})), 2)
        self.assertEquals(b.findParent('ul')['a'], 'b')

    PROXIMITY_TEST = BeautifulSoup('<b id="1"><b id="2"><b id="3"><b id="4">')
        
    def testNext(self):
        soup = self.PROXIMITY_TEST
        b = soup.first('b', {'id' : 2})
        self.assertEquals(b.findNext('b')['id'], '3')
        self.assertEquals(b.firstNext('b')['id'], '3')
        self.assertEquals(len(b.fetchNext('b')), 2)
        self.assertEquals(len(b.fetchNext('b', {'id' : 4})), 1)

    def testPrevious(self):
        soup = self.PROXIMITY_TEST
        b = soup.first('b', {'id' : 3})
        self.assertEquals(b.findPrevious('b')['id'], '2')
        self.assertEquals(b.firstPrevious('b')['id'], '2')
        self.assertEquals(len(b.fetchPrevious('b')), 2)
        self.assertEquals(len(b.fetchPrevious('b', {'id' : 2})), 1)


    SIBLING_TEST = BeautifulSoup('<blockquote id="1"><blockquote id="1.1"></blockquote></blockquote><blockquote id="2"><blockquote id="2.1"></blockquote></blockquote><blockquote id="3"><blockquote id="3.1"></blockquote></blockquote><blockquote id="4">')                          

    def testNextSibling(self):
        soup = self.SIBLING_TEST
        tag = 'blockquote'
        b = soup.first(tag, {'id' : 2})
        self.assertEquals(b.findNext(tag)['id'], '2.1')
        self.assertEquals(b.findNextSibling(tag)['id'], '3')
        self.assertEquals(b.firstNextSibling(tag)['id'], '3')
        self.assertEquals(len(b.fetchNextSiblings(tag)), 2)
        self.assertEquals(len(b.fetchNextSiblings(tag, {'id' : 4})), 1)

    def testPreviousSibling(self):
        soup = self.SIBLING_TEST
        tag = 'blockquote'
        b = soup.first(tag, {'id' : 3})
        self.assertEquals(b.findPrevious(tag)['id'], '2.1')
        self.assertEquals(b.findPreviousSibling(tag)['id'], '2')
        self.assertEquals(b.firstPreviousSibling(tag)['id'], '2')
        self.assertEquals(len(b.fetchPreviousSiblings(tag)), 2)
        self.assertEquals(len(b.fetchPreviousSiblings(tag, {'id' : 1})), 1)

    def testTextNavigation(self):
        soup = BeautifulSoup('Foo<b>Bar</b><i id="1"><b>Baz<br />Blee<hr id="1"/></b></i>Blargh')
        baz = soup.firstText('Baz')
        self.assertEquals(baz.findParent("i")['id'], '1')
        self.assertEquals(baz.findNext(text='Blee'), 'Blee')
        self.assertEquals(baz.findNextSibling(text='Blee'), 'Blee')
        self.assertEquals(baz.findNextSibling(text='Blargh'), Null)
        self.assertEquals(baz.findNextSibling('hr')['id'], '1')

class SiblingRivalry(SoupTest):
    "Tests the nextSibling and previousSibling navigation."

    def testSiblings(self):
        soup = BeautifulSoup("<ul><li>1<p>A</p>B<li>2<li>3</ul>")
        secondLI = soup.first('li').nextSibling
        self.assert_(secondLI.name == 'li' and secondLI.string == '2')
        self.assertEquals(soup.firstText('1').nextSibling.name, 'p')
        self.assertEquals(soup.first('p').nextSibling, 'B')
        self.assertEquals(soup.first('p').nextSibling.previousSibling.nextSibling,
                          'B')

class TagsAreObjectsToo(SoupTest):
    "Tests the various built-in functions of Tag objects."

    def testLen(self):
        soup = BeautifulSoup("<top>1<b>2</b>3</top>")
        self.assertEquals(len(soup.top), 3)

class StringEmUp(SoupTest):
    "Tests the use of 'string' as an alias for a tag's only content."

    def testString(self):
        s = BeautifulSoup()
        s.feed("<b>foo</b>")
        self.assertEquals(s.b.string, 'foo')

    def testLackOfString(self):
        s = BeautifulSoup()
        s.feed("<b>f<i>e</i>o</b>")
        self.assert_(not s.b.string)

class ThatsMyLimit(SoupTest):
    "Tests the limit argument."

    def testBasicLimits(self):
        s = BeautifulSoup('<br id="1" /><br id="1" /><br id="1" /><br id="1" />')
        self.assertEquals(len(s.fetch('br')), 4)
        self.assertEquals(len(s.fetch('br', limit=2)), 2)
        self.assertEquals(len(s('br', limit=2)), 2)
        
class KeepOnParsing(SoupTest):

    """Verifies that the parser treats multiple feed() calls the same
    as one big feed() call only if constructed with
    initialTextIsEverything=False."""

    def testMultipleParseCalls(self):
        f1 = '<foo>bah<bar>'
        f2 = 'blee</bar></foo>'        

        s1 = BeautifulSoup(f1+f2)
        s2 = BeautifulSoup(f1)
        s2.feed(f2)
        s3 = BeautifulSoup(f1, initialTextIsEverything=False)
        s3.feed(f2)
        self.assertNotEqual(s1, s2)
        self.assertEqual(s1, s3)

class UnicodeRed(SoupTest):
    "Makes sure Unicode works."

    def setUp(self):    
        text = 'foo<b>bar</b>'
        self.soup = BeautifulStoneSoup(initialTextIsEverything=False)
        self.soup.feed(text)

    def testBasicUnicode(self):
        import types
        sType = types.StringType
        uType = types.UnicodeType
        
        u = u'\3100'
        #It starts out ASCII...
        self.assertEqual(type(self.soup.renderContents()), sType)
        self.assertEqual(type(self.soup.prettify()), sType)
        #But you can have unicode if you want.
        self.assertEqual(type(unicode(self.soup)), uType)

        #Add a Unicode character and it's Unicode.
        self.soup.feed("<b>" + u + "</b>")
        self.assertEqual(type(self.soup.renderContents()), uType)
        self.assertEqual(type(self.soup.prettify()), uType)
        #What did I mean when I wrote this? It doesn't make sense anymore! -LR
        #But you can have ASCII if you want.
        #self.assertEqual(type(str(self.soup)), sType)

        #The part without any Unicode is still ASCII.
        self.assertEqual(type(self.soup.b.prettify()), sType)

        #But if you add a Unicode character it'll become Unicode.
        self.soup.b['foo'] = u'\3100'
        self.assertEqual(type(self.soup.b.prettify()), uType)

class WeHaveNullBananas(SoupTest):
    "Makes sure Null works as advertised."
    def testNull(self):
        soup = BeautifulSoup("<b>Foo</b>")
        self.assertNotEqual(soup.first('b'), Null)
        self.assertEqual(soup.first('a'), Null)
        self.assertEqual(soup.first('b').parent.parent.parent, Null)
        self.assertEqual(soup.first('b').parent.parent[soup.blah].parent.thisIsntEvenARealMember().parent, Null)
        self.assertEqual(soup.bTag.nosuchTag, Null)
        self.assertEqual(soup.firstText('no such text'), Null)                         
class WriteOnlyCode(SoupTest):
    "Testing the modification of the tree, such as it is."

    def testReplaceContents(self):
        soup = BeautifulSoup('<a>foo</a>')
        soup.a.contents[0] = (NavigableString('bar'))
        self.assertEqual(soup.renderContents(), '<a>bar</a>')

    def testModifyAttributes(self):
        soup = BeautifulSoup('<a id="1"></a>')
        soup.a['id'] = 2
        self.assertEqual(soup.renderContents(), '<a id="2"></a>')
        del(soup.a['id'])
        self.assertEqual(soup.renderContents(), '<a></a>')
        soup.a['id2'] = 'foo'
        self.assertEqual(soup.renderContents(), '<a id2="foo"></a>')

    def testNewTagCreation(self):
        "Makes sure tags don't step on each others' toes."
        a = Tag('a')
        ol = Tag('ol')
        a['href'] = 'http://foo.com/'
        self.assertRaises(KeyError, lambda : ol['href'])

class QuoteMeOnThat(SoupTest):
    "Test quoting"

    def testLiteralMode(self):
        text = "<script>if (i<imgs.length)</script><b>Foo</b>"
        soup = BeautifulSoup(text)
        self.assertEqual(soup.script.contents[0], "if (i<imgs.length)")
        self.assertEqual(soup.b.contents[0], "Foo")

class OperatorOverload(SoupTest):
    "Our operators do it all! Call now!"

    def testTagNameAsFirst(self):
        "Tests that referencing a tag name as a member delegates to first()."
        soup = BeautifulSoup('<b id="1">foo<i>bar</i></b><b>Red herring</b>')
        self.assertEqual(soup.b.i, soup.first('b').first('i'))
        self.assertEqual(soup.b.i.string, 'bar')
        self.assertEqual(soup.b['id'], '1')
        self.assertEqual(soup.b.contents[0], 'foo')
        self.assert_(not soup.a)

        #Test the .fooTag variant of .foo.
        self.assertEqual(soup.bTag.iTag.string, 'bar')
        self.assertEqual(soup.b.iTag.string, 'bar')
        self.assertEqual(soup.first('b').first('i'), soup.bTag.iTag)
        
class NestableEgg(SoupTest):
    """Here we test tag nesting. TEST THE NEST, DUDE! X-TREME!"""

    def testParaInsideBlockquote(self):
        soup = BeautifulSoup('<blockquote><p><b>Foo</blockquote><p>Bar')
        self.assertEqual(soup.blockquote.p.b.string, 'Foo')
        self.assertEqual(soup.blockquote.b.string, 'Foo')
        self.assertEqual(soup.first('p', recursive=False).string, 'Bar')

    def testNestedTables(self):
        text = """<table id="1"><tr><td>Here's another table:
        <table id="2"><tr><td>Juicy text</td></tr></table></td></tr></table>"""
        soup = BeautifulSoup(text)
        self.assertEquals(soup.table.table.td.string, 'Juicy text')
        self.assertEquals(len(soup.fetch('table')), 2)
        self.assertEquals(len(soup.table.fetch('table')), 1)
        self.assertEquals(soup.first('table', {'id' : 2}).parent.parent.parent.name,
                          'table')

        text = "<table><tr><td><div><table>Foo</table></div></td></tr></table>"
        soup = BeautifulSoup(text)
        self.assertEquals(soup.table.tr.td.div.table.contents[0], "Foo")

        text = """<table><thead><tr>Foo</tr></thead><tbody><tr>Bar</tr></tbody>
        <tfoot><tr>Baz</tr></tfoot></table>"""
        soup = BeautifulSoup(text)
        self.assertEquals(soup.table.thead.tr.contents[0], "Foo")

    def testBadNestedTables(self):
        soup = BeautifulSoup("<table><tr><table><tr id='nested'>")
        self.assertEquals(soup.table.tr.table.tr['id'], 'nested')

class CleanupOnAisleFour(SoupTest):
    """Here we test cleanup of text that breaks SGMLParser or is just
    obnoxious."""

    def testSelfClosingtag(self):
        self.assertEqual(str(BeautifulSoup("Foo<br/>Bar").first('br')),
                         '<br />')

        self.assertSoupEquals('<p>test1<br/>test2</p>',
                              '<p>test1<br />test2</p>')

    def testWhitespaceInDeclaration(self):
        self.assertSoupEquals('<! DOCTYPE>', '<!DOCTYPE>')

    def testJunkInDeclaration(self):
        self.assertSoupEquals('<! Foo = -8>a', '<!Foo = -8>a')

    def testIncompleteDeclaration(self):
        self.assertSoupEquals('a<!b <p>c')

    #def testValidButBogusDeclarationFAILS(self):
    #    self.assertSoupEquals('<! Foo >a', '<!Foo >a')

    #def testIncompleteDeclarationAtEndFAILS(self):
    #    self.assertSoupEquals('a<!b')

    def testSmartQuotesNotSoSmartAnymore(self):
        self.assertSoupEquals("\x91Foo\x92", '&lsquo;Foo&rsquo;')        
        
if __name__ == '__main__':
    unittest.main()
