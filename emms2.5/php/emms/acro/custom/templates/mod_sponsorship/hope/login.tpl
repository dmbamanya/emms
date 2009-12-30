<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-3">
<title>{html_title_client_name}</title>
<style type='text/css'>
body {
  margin: 0px 0px 0px 0px;
  }
body {
  color : gray ;
  font-family: verdana;
  text-decoration : none;
  font-size: 16px;
  font-weight: light; 
  }	
img {
  margin-top:4px;
  margin-bottom:4px;
  border-style: solid;
  border-width: 0px; 
  }
  
.qform {
  width: 150px;
  font-family : sans-serif; 
  font-size : 11px; 
  font-style : normal; 
  font-weight : normal; 
  text-decoration : none; 
  color : black; 
  background-color: #ffffff;
  border-style: solid;
  border-width:1px;
  border-color:#DFB53D;
  margin: 1px 0px 0px 0px;
  } 
  
a {
  color : #703827;
  font-family: verdana;
  text-decoration : none;
  font-size: 11px;
  font-weight: bold;     
  }	
hr { 
  height: 3px; 
  border-style: solid; 
  border-color: #44576A;
  color: #44576A;
  background: #44576A;
  margin-top:'0px';
  margin-bottom:'0px';
  padding-top:'0px';
  padding-bottom:'0px';
  } 
td.logo {
  background-color: #3A4C5F;
  padding: 0px 0px 0px 30px;
  height: 115px;
  }
table.login {
  background-repeat : no-repeat; 
  background-attachment : fixed; 
  background-position: 0 120;	
  }
td.welcome {
  color : #698D54;
  font-family: verdana;
  font-size: 16px;
  font-weight: bold; 
}
td.sponsorship_program_name {
  color : #DFB53D;
  font-family: verdana;
  font-size: 22px;
  font-weight: normal;
}
td.contact {
  color : black;
  font-family: verdana;
  font-size: 10px;
  font-weight: normal; 
  }
td.copyright {
  color : black;
  font-family: verdana;
  font-size: 10px;
  font-weight: normal; 
  text-align: left;
  height: 40px;
  }
td.languages {
  text-align: right;
  padding: 0px 0px 0px 0px;
  height: 20px;
  }
td.login {
  color : #A3BF93;
  font-family: verdana;
  text-decoration : none;
  font-size: 12px;
  font-weight: light; 
  }		
.msg {
  color : #990000;
  font-family: verdana;
  text-decoration : none;
  font-size: 11px;
  font-weight: normal; 
  }
</style>
</head>
<body onLoad='javascript:document.loginForm.screenWidth.value=window.screen.width'>
<br /><br />
<table cellspacing=0 cellpadding=0 class=login>
  <tr>
    <td>
      <table cellspacing=2 cellpadding=0 border=0>
        <tr>
          <td rowspan=6 width='100px'></td>
          <td class=sponsorship_program_name>Bank of HOPE: sponsorship account</td>
        </tr>
	<tr><td></td></tr>
        <tr>
          <td class=languages>
            <a href='index.php?lang=es&partner={partner}'>Espa&ntilde;ol</a>
	          <a>|</a>
	          <a href='index.php?lang=en&partner={partner}'>English</a>
	          <a>|</a>
	          <a href='index.php?lang=fr&partner={partner}'>Francais</a>
          </td>
        </tr>
	<tr><td></td></tr>
        <tr>
          <td>
	        <br>
	        <span class=msg>{message}</span>
	        <form {loginForm_attributes}>
 	        {loginForm_screenWidth_html}
 	        {loginForm_lang_html}
                {loginForm_partner_html}
	        <table border=0 cellspacing=0 cellpadding=0 width=250px>
	          <tr>
		        <td width=105px class=login>{loginForm_username_label}</td>
		        <td>{loginForm_username_html}</td>
	          </tr>
	          <tr>
		        <td class=login>{loginForm_password_label}</td>
		        <td>{loginForm_password_html}</td>
	          </tr>
	          <tr>
		        <td colspan=2 align=right><br>{loginForm_submit_html}</td>
	          </tr>
	        </table>
          </td>
        </tr>
        <tr>
           <td class=copyright><b>Powered by e-MMS</b><br />{copyright}<br><br></td>
        </tr>
      </table>
    </td>
  </tr>
</table>
</body>
</html>