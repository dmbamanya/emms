<table>
  <tr><td colspan=2><h1>{rpt_label}</h1></td></tr>
  <tr><td class=label>{date_range_label}</td><td>{date_range}</td></tr>
</table>
<br />
{chart}


<div style='width: 300px; position: relative; left: 200px; float: left;'>
<table>
  <tr><td align=right>(+) {balance_from}</td></tr>
  <tr><td align=right>(+) {disbursement}*</td></tr>
  <tr><td align=right>(-) {recovery}*</td></tr>
  <tr><td align=right>(-) {write_off}*</td></tr>
  <tr><td align=right>-----------------------------------------</td></tr>
  <tr><td align=right>= {result}</td></tr>
</table>
</div>

<div style='width: 300px; position: relative; left: 200px;'>
<table>
  <tr><td align=right>(+) {balance_to}</td></tr>
  <tr><td align=right>(-) {result}</td></tr>
  <tr><td align=right>-----------------------------------------</td></tr>
  <tr><td align=right>= {diff}</td></tr>
</table>
</div>

<br /><br /><br /><br />
<div>
*{releasedInPeriod}
</div>