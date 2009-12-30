<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd"> 
<html>
<title>{html_title_client_name}</title>
<head>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-3">
{loginForm_javascript}
<LINK href='themes/blue/style.login.css' rel='stylesheet' type='text/css'>  
</head>
<body onLoad='javascript:document.loginForm.screenWidth.value=window.screen.width' background="./themes/blue/logo-watermark.png">

<div class='imf'>{html_title_client_name}</div>

    <div class='form'>
      <table cellspacing=5 cellpadding=0>
        <tr><td><div class='logo'><img class=logo src='themes/blue/loginlogo.png' alt=''></div></td></tr>
        <tr><td><div class='welcome'>{welcome}<hr></div></td></tr>
        <tr><td><div class='contact'>{contact}</div></td></tr>
        <tr>
          <td>
	        <br>
	        <span class=msg>{message}</span>
	        <form {loginForm_attributes} onKeyDown="if (event.keyCode == 13) document.loginForm.submit();">
 	        {loginForm_screenWidth_html}
 	        {loginForm_lang_html}
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
        <tr><td></td></tr>
      </table>
    </div>

<div class='language'>
  <a href='index.php?lang=es'>Espa&ntilde;ol</a>
  <a>|</a>
  <a href='index.php?lang=en'>English</a>
  <a>|</a>
  <a href='index.php?lang=fr'>Francais</a>
</div>

<div class='copyright'><hr>{copyright}<br />{emms_version}</div>

</body>
</html>