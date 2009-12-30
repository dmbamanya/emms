<script>{refreshParent}</script>
<h1>{form_name}</h1>
<b><font color='red'>{msg_label}:</font> {msg_text}</b>
<br><br>
{revisionForm_javascript}
<form {revisionForm_attributes}>
{revisionForm_scr_name_html}
{revisionForm_id_html}
<table>
  <tr>
    <td class=label>{zone_label}</td>
    <td>{zone}</td>
  </tr>
  <tr>
    <td class=label>{borrower_label}</td>
    <td>{borrower}</td>
  </tr>
  <tr>
    <td class=label>{borrower_type_label}</td>
    <td>{borrower_type}</td>
  </tr>
  <tr>
    <td class=label>{loan_type_label}</td>
    <td>{loan_type}</td>
  </tr>
  <tr>
    <td class=label>{amount_label}</td>
    <td>{amount}</td>
  </tr>
  <tr>
    <td class=label>{check_status_label}</td>
    <td>{check_status}</td>
  </tr>
  <tr>
    <td class=label>{xp_delivered_date_label}</td>
    <td>{xp_delivered_date}</td>
  </tr>
  <tr>
    <td class=label>{xp_first_payment_date_label}</td>
    <td>{xp_first_payment_date}</td>
  </tr>
  <tr>
    <td colspan=2 class=label>{chart_title}</td>
  </tr>
  <tr>
    <td colspan=2><br>{chart}<br /><br /></td>
  </tr>
  <tr>
    <td class=label>{revisionForm_memo_label}</td>
    <td>{revisionForm_memo_html}</td>
  </tr>
  <tr>
    <td colspan=2 align=right><br />{revisionForm_submit_html}</td>
  </tr>
  <tr>
    <td class=label>{more_options}</td>
  </tr>
  <tr>
    <td colspan=2><a href='index.popup.php?scr_name=LN.SCR.doRejectRequest&id={master_id}'>1. {clickToRejectRequest}</a></td>
  </tr>
  <tr>
    <td colspan=2><a href='index.popup.php?scr_name=LN.SCR.doRetractRequest&id={master_id}'>2. {clickToRetractRequest}</a></td>
  </tr>
</table>
</form>
