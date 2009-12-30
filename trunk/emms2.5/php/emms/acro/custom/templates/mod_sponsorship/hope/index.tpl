<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<title>{html_title_client_name}</title>
<head>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-3">
{loginForm_javascript}
<style type='text/css'>
  body { margin: 0px; font-family:verdana; font-size:10px; }
  h1 { color:#4885A2; font-size:14px; font-weight:bold; margin:5px 0px 0px 0px; }
  h2 { color:#703827; font-size:11px; font-weight:bold; margin: 10px 0px 10px 0px; padding: 0;}
  hr { height:2px; border-style:solid; border-color:#6699CC;background:#6699CC;color:#6699CC; margin: 0px; padding: 0px; }
  hr.dash { height:1px; border-style:dashed; border-color:#CCCCCC; padding:'0'; margin:'0';}
  img { border-style:solid; border-width:0px; }
  img.record { border-width:0px; border-color:black; width:200px; margin:0px 0px 10px 30px;}
  table { font-size:10px; border-width:0; font-size:11px; margin-left:0px; }
  table.chart { background-color:#E5EBFB; }
  tr.rowOff {background-color:white;}
  tr.rowOn {background-color:#FFFF99;}
  td, th { padding:0; font-size:10px;}
  td.header { background-color:#68A0BC; font-weight:bold; color:white; padding: 2px 10px 2px 10px; text-align:center; }
  td.label { font-weight:bold; color:#336699; padding: 4px 20px 4px 0px; text-align:left; vertical-align:top; }
  td.sublabel { font-weight:normal; color:#336699; padding: 4px 20px 4px 10px; text-align:left; vertical-align:top; }
  td.list { padding: 4px 20px 4px 0px; vertical-align:top; }
  td.chart { background-color:white; padding:2px 5px 2px 5px; }
  td.activeChart { padding:2px 5px 2px 5px; }
  td.tabmenu { background-color:#99B7D3; padding:4px 20px 4px 20px; }
  td.treemenu { border-style:solid; border-color:#97AD7E; border-width:0px; background-color:#E5EBFB; }
  td.treemenulabel { color:#68A0BC; font-size:20px; font-weight:normal; padding-bottom: 10px;}
  td.treemenucontainer { background-color:#E5EBFB;  padding:0px 5px 0px 5px; }
  td.sponsorship_program_name {color : #DFB53D;font-family: verdana;font-size: 22px;font-weight: normal;}
  td.sponsor_name { color:#703827;font-family:verdana;font-size:13px;font-weight: normal;padding:5px 0px 20px 0px; }
  td.sponsor_contact { color : font-family: verdana;font-size: 10px;font-weight: bold;padding:0px 0px 0px 10px; }
  td.coloredborder1 { border-style:outset; border-color:green; border-width:5px; padding:0px 30px 0px 0px; }
  td.coloredborder2 { border-style:outset; border-color:yellow; border-width:5px; padding:0px 30px 0px 0px; }
  td.coloredborder3 { border-style:outset; border-color:blue; border-width:5px; padding:0px 30px 0px 0px; }
  caption { color:#68A0BC; padding:0px; margin: 4px 0px 4px 0px; font-weight:bold; font-size:12px; text-align:left;}
  textarea { width:300px; }
  input { width:150px;}
  input.large { width:300px; }
  input.txt { border-style: none; background-color: white; font-size:10px; font-family: verdana; font-weight: normal; color: black;}
  input.txtBold { border-style: none; background-color: white; font-size:10px; font-family: verdana; font-weight: bold; color: black;}
  input.radio { width:25px; }
  input.checkbox { width:25px; vertical-align:bottom; margin: 0px 5px -2px 5px;}
  select { width:156px;  margin-top:1px; margin-bottom:1px;}
  select.large { width:306px; }
  a { text-decoration:none; color:#46987A; }
  a.visited { color:#46583A; }
  a:hover { color:#68A0BC; }
  a.tabmenu { color: white; }
  a.tabmenu:hover { color: white; }
  a.record { color: #5E9D5E; }
  a.record:hover { color: #68A0BC; }
  .MFIname { font-family:verdana; color:#65829F; font-size:11px; font-weight:bold; }
  .welcomeUser { color:#0852a5; font-weight:bold; }
  .content { padding:0px 0px 0px 10px; }
  .path { padding:10px 10px 10px 0px; }
  .pathlabel { color:#0852a5; font-weight:bold; padding:0px 10px 0px 0px; }
  .info { padding:10px 10px 10px 0px; }
  .infolabel { color:#5E9D5E; font-weight:bold; padding:0px 10px 0px 0px; }
  .error { color:#CC3333; font-weight:bold; font-size:10px; }
  .required { color:#6697D1; }
</style>
<script language="JavaScript" type="text/javascript" src="../app/CS.functions.js"></script>
</head>
<body onLoad='this.focus()'>

<table width='95%' cellspacing='0' cellpadding='0' style='margin: 20px'>
  <tr>
    <td valign='top' width='80px'>
      <table>
        <tr>
	  <td align=center>
            <br /><br /><br /><a href='index.php'><img alt='' src='http://localhost/projects/resources/ICONS/hope/home.png'><br />Home</a>
	  </td>
	</tr>
	<tr>
	  <td align=center>
            <br /><br /><a href='index.php?logout=1'><img alt='' src='http://localhost/projects/resources/ICONS/hope/logout.png'><br />Log Off</a>
	  </td>
	</tr>
      </table>
    </td>
    <td>
      <table border=0 cellspacing='0' cellpadding='0'>
        <tr>
          <td class=sponsorship_program_name>Bank of Hope: sponsorship account</td>
        </tr>
        <tr>
          <td class=sponsor_name>Account: {sponsor_name}</td>
        </tr>
        <tr>
          <td>
	    {message}
            <h1>{content_title}</h1>
	    {content}
            <br /><br />
          </td>
        </tr>
      </table>
    </td>
  </tr>
</table>