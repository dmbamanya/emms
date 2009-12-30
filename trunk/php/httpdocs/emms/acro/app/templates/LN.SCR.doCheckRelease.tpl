<script>{refreshParent}</script>
<h1>{form_name}</h1>
<b><font color='red'>{msg_label}:</font></b> {msg_text}
{releaseForm_javascript}
<form {releaseForm_attributes}>
{releaseForm_scr_name_html}
{releaseForm_id_html}
{releaseForm_min_delivered_date_html}
{releaseForm_max_first_payment_date_html}
<br />
<table cellpadding=0 cellspacing=0>
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
    <td class=label>{check_number_label}</td>
    <td>{check_number}</td>
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
    <td class=label colspan=2>{chart_title}</td>
  </tr>
  <tr>
    <td colspan=2>{chart}<br /><br /></td>
  </tr>
  <tr>
    <td class=label>{releaseForm_delivered_date_label}</td>
    <td>{releaseForm_delivered_date_html}</td>
  </tr>
  <tr>
    <td class=label>{releaseForm_first_payment_label}</td>
    <td>{releaseForm_first_payment_html}</td>
  </tr>
  <tr>
    <td class=label>{releaseForm_memo_label}</td>
    <td>{releaseForm_memo_html}</td>
  </tr>
  <tr>    
    <td colspan=2 align=right><br />{releaseForm_submit_html}</td>
  </tr>
  <tr>
    <td class=label>{more_options}</td>
  </tr>
  <tr>
    <td colspan=2><a href='index.popup.php?scr_name=LN.SCR.doRetractRequest&id={master_id}'>1. {clickToRetractRequest}</a></td>
  </tr>
</table>
</form>