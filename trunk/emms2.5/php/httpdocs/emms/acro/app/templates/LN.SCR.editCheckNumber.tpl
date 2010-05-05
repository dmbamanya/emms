<script>{refreshParent}</script>
<h1>{form_title}</h1>
<b><font color='red'>{msg_label}:</font> {msg_text}</b>
<br /><br /><br />
<form {editCheckNumberForm_attributes}>
{editCheckNumberForm_scr_name_html}
{editCheckNumberForm_id_html}

      <table cellpadding=0 cellspacing=0>
        <tr>
           <td class=label>{borrower_label}</td>
           <td>{borrower}</td>
        </tr>
        <tr>
           <td class=label>{amount_label}</td>
           <td>{amount}</td>
        </tr>
        <tr>
           <td class=label valign=top>{editCheckNumberForm_check_number_label}</td>
           <td valign=top>{editCheckNumberForm_check_number_html}</td>
        </tr>
        <tr>
           <td valign=top></td>
           <td valign=top><br />{editCheckNumberForm_submit_html}</td>
        </tr>
      </table>

</form>