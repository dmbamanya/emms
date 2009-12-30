<!-- BEGIN html --> 
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd"> 
<html>
<head>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-3">
<title>{html_title_client_name}</title>
<link href='{theme}style.css' rel='stylesheet' type='text/css'>
<script type="text/javascript" language="JavaScript" src="javascript/TreeMenu.js"></script>
<script type="text/javascript" language="JavaScript" src="CS.functions.js"></script>
</head>
<body>
<table border=0 width='{screenWidth}px' cellspacing='0' cellpadding='0'>
  <tr>
    <td>
      <table border=0 cellspacing='0' cellpadding='0' width='100%'>
        <tr>
		  <td width='200px' align=center><img style='margin-top:8px;margin-bottom:8px;' src='{theme}logo.png' border='0'></td>
		  <td valign=bottom>
		    <table border=0 cellspacing='0' cellpadding='0' width='100%'>
			  <tr>
				<td align=right valign=bottom class=MFIname>{client_name}</td>
			  </tr>
			  <tr>
				<td align=right>{greetings}<br><br></td>
			  </tr>
			  <tr>
				<td align=right>
				  <!-- BEGIN tabmenu --> 
				    <table cellpadding='3px' cellspacing='1px'>
					  <tr>
					    <!-- BEGIN tabitem --> 
					      <td class=tabmenu><a class=tabmenu href='{URL}'>{TAB}</a></td>
					    <!-- END tabitem --> 
					  </tr>
					</table>				     
				  <!-- END tabmenu --> 
				</td>
			  </tr>
			</table>
		  </td>
	    </tr>
		<tr>
		  <td></td>
		</tr>
	  </table>
	</td>
  </tr>
  <tr>
	<td class=treemenucontainer height=3px></td>
  </tr>
  <tr>
	<td>
	  <table border=0 cellspacing="0" cellpadding="0" width='100%'>
	    <tr>
		  <td class=treemenucontainer width=200px valign=top>
		    <table cellpadding="5" width='100%'>
		      <tr>
				<td class=treemenu height='300px' valign=top><br /><br /><br />{navtree}<br /></td>
			  </tr>
			</table>
			<br>
		  </td>		  
		  <td valign=top align=left width='{screenWidth}px' class=content>
		    <div class=path><span class=pathlabel>{path_caption} </span>{path}</div>
		    {message}
		    <h1>{content_title}</h1>
		    {content}
		  </td>
		</tr>
	  </table>
	</td>
  </tr>
</table>
<!-- END html --> 