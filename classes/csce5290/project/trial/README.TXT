
/* Trial data for the WePS task at Semeval 2007 */

WePS (http://nlp.uned.es/pergamus/pubs/p066-artiles.pdf) is a corpus of web pages retrieved using 
person names as queries to web search engines. 
Those pages were manually clustered according to the specific individual referred in each page.

Names in the trial dataset:

Ann Hill, Brenda Clark, Christine King, Helen Miller, Lisa Harris, Mary Johnson, Nancy Thompson, Samuel Baker, Sarah Wilson


Contents:

web_data/

  Information collected from the Google API during the web page retrieval. 
  For each page it includes: position in the ranked results, URL, title, text snippet, related
  web pages (using Google's "related:"), and pages linking this one.

web_pages/

  All the files have been encoded in ISO-8859-1.
  Each file includes all the web pages downloaded for a person name, according to the first 100
  hits in the Google API for that name.

  The only modification performed in the HTML code has been the addition of <span> tags indicating
  the occurence of the following semanting information related to the specific person referred to in the page:
  - name, job, author of, deﬁnition, date of birth, date of death, image, place of birth, email, location, 
    phone no., address, place of death, fax no..

  The <span> tag contains a "class" attribute containing the type of information and an "id" attribute.
  For instance:
  <span class="name" tag_id="0.96746800 1107440476">Christine King</span>

  Note that this information is tagged only for the ambiguous name in each document.
  IMORTANT: these semantic annotations will not be provided in the final training and testing sets for the Semeval'07 WePS Task.

clustering/

  Contains the manual clustering of the web pages, according to the identified individuals.


For more information about the corpus (including baseline clustering results):
• J. Artiles, J. Gonzalo, F. Verdejo, 2005. A Testbed for People Searching Strategies in the WWW . 
  SIGIR 2005 Conference. Special Interest Group on Information Retrieval.

