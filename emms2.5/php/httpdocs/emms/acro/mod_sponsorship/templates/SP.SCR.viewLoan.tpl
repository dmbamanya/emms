<table border=0 style="position: relative; top:-30px;">
<tr>
<td valign=top class=left>
    <h1>{bde_name}</h1>
    <font class=blue>{branch} - {country}<br /><br />
    <b>Loan:</b> ${amount}<br />
    <b>Date:</b> {delivered_date}<br />
    <br />
    <br />
<!-- BEGIN bdeloans -->
  <table cellpadding='0px' cellspacing='0px' width='100%'>
    <tr>
    <!-- BEGIN loanrecord -->
    <td width=200px>
      <img width=180px src='{cli_photo}' style='margin: 0 14px 4px 0;'>
    </td>
    <td valign=top>
      <h4>{cli_name}.</h4>
      <div class='hr'></div>
      {cli_gender}, {cli_age} years old, {cli_dependants} dependants, {cli_education} <br />
      Joined Esperanza in {cli_creator_date}<br /><br />
      <b>Business:</b> {business}<br /><br />
      <b>Loan:</b> ${loan_amount}<br />
      <b>{pmt_label}:</b> ${pmt} - <b>{penalties_label}:</b> ${penalties} - <b>{balance_kp_label}:</b> ${balance_kp}
    </td>
    <!-- END loanrecord -->
  </tr>
  <tr><td>&nbsp;</td></tr>
</table>
<!-- END bdeloans -->
</td>
</tr>
</table>
<style>
body {background-color: #154463;}
h4 { margin: 0px 0px 5px 0px; }
td.content { background-color: white; }
td.sponsor_contact { padding-bottom: 20px; }
td.MFIname { visibility: hidden; }
td.sponsorship_program_name { color: white; }
td.sponsor_name { color: #cccccc; }
td.sponsor_contact { color: #cccccc; }
table { width: 100%; }
div.hr { height:1px; background-color: #ccc; width: 450px; margin: 0px 0px 10px 0px; } 
</style>
