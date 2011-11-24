<script>{refreshParent}</script>
<h1>{form_name}</h1>
<b><font color='red'>{msg_label}:</font></b> {msg_text}
{disbursementForm_javascript}
<form {disbursementForm_attributes}>
{disbursementForm_scr_name_html}
{disbursementForm_id_html}
<br><br>
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
    <td class=label>{master_img_label}</td>
    <td>{master_img}</td>
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
    <td class=label colspan=2>{chart_title}</td>
  </tr>
  <tr>
    <td colspan=2>{chart}<br /><br /></td>
  </tr>
  <tr>
    <td class=label>{disbursementForm_check_number_label}</td>
    <td>{disbursementForm_check_number_html}</td>
  </tr>
  <tr>
    <td class=label>{disbursementForm_fund_label}</td>
    <td>{disbursementForm_fund_html}</td>
  </tr>
  <tr>
    <td class=label>{disbursementForm_sponsor_id_label}</td>
    <td>{disbursementForm_sponsor_id_html}</td>
  </tr>
  <tr>
    <td class=label>{disbursementForm_memo_label}</td>
    <td>{disbursementForm_memo_html}</td>
  </tr>
  <tr>
    <td colspan=2 align=right><br />{disbursementForm_submit_html}</td>
  </tr>
  <tr>
    <td class=label>{more_options}</td>
  </tr>
  <tr>
    <td colspan=2><a href='index.popup.php?scr_name=LN.SCR.doRetractRequest&id={master_id}'>1. {clickToRetractRequest}</a></td>
  </tr>
</table>
</form>

<br><br>
