<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!-- BEGIN Top Stories -->
<table border="0" cellpadding="0" cellspacing="0" width="443">
   <tr>
       <td class="v14-header-1">Top stories</td>
   </tr>
   <tr>
   <sql:query var="rs" dataSource="jdbc/IBMDB">
       SELECT * FROM news
           WHERE (publish IS TRUE) AND (start_date <= CURDATE()) AND (end_date >= CURDATE())
   </sql:query>

       <td>
           <table border="0" cellpadding="0" cellspacing="0" class="v14-gray-table-border">
               <tr>
                   <td width="443">
                       <table border="0" cellpadding="0" cellspacing="0" width="430">
                       <c:forEach var="row" items="${rs.rows}">
                           <tr valign="top">
                               <td align="right" width="18" class="ipt"><img alt="" height="16" src="//www.ibm.com/i/v14/icons/fw_bold.gif" width="16"/></td>
                               <td class="nlbp">
                                   <p><b><a class="fbox" href="${row.link}">${row.headline}</a></b>
                                   ${row.summary}</p>
                               </td>			
                           </tr>
                       </c:forEach>
                           <tr>
                               <td colspan="2"><img src="i/c.gif" border="0" height="5" width="1" alt=""/></td>
                           </tr>
                           <tr>
                               <td colspan="2"><img src="i/c.gif" border="0" height="5" width="1" alt=""/></td>
                           </tr>
                       </table>
                   </td>
               </tr>
           </table>
       </td>
   </tr>
</table>
<!-- END Top Stories -->