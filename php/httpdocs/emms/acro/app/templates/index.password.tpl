<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head><META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-3">
<title>{html_title_client_name}</title>
<link href='{theme}style.login.css' rel='stylesheet' type='text/css'>
</head>

<body background="./themes/blue/logo-watermark.png">
<div class='imf'>{html_title_client_name}</div>
<div class='form'>
  <table cellspacing=5 cellpadding=0>
    <tr><td><div class='logo'><img class=logo src='themes/blue/loginlogo.png' alt=''></div></td></tr>
    <tr><td><div class='welcome'>{welcome}<hr></div></td></tr>
    <tr><td><div><span class='msg'><b>{expired}</b><br />{memorize}</span></div></td></tr>
    <tr>
      <td>
        <br>
        <form {userForm_attributes} onKeyDown="if (event.keyCode == 13) document.userForm.submit();">
        <table border=0 cellspacing=0 cellpadding=0>
          <tr>
            <td width=105px class=login>{userForm_old_password_label}</td>
            <td>{userForm_old_password_html}</td>
          </tr>
          <tr>
            <td width=105px class=login>{userForm_password_label}</td>
            <td>{userForm_password_html}</td>
          </tr>
          <tr>
            <td class=login>{userForm_verify_label}</td>
            <td>{userForm_verify_html}</td>
          </tr>
          <tr>
            <td colspan=2 align=right><br>{userForm_submit_html}</td>
          </tr>
        </table>
      </td>
    </tr>
    <tr><td></td></tr>
  </table>
</div>
</body>
</html>