<table width=80% border=0 align=center cellpadding=0 cellspacing=0 class=n02>
<tr>
<td width=25%></td>
<td width=25%></td>
<td width=25%></td>
<td width=25%></td>
</tr>
<tr>
<td colspan=4 align=center>
	<b><?= WEBPAGE::$conf['app']['client_name']; ?></b>
	<br>
	<?= $zone; ?></td>
</tr>
<tr>
<td colspan=4><hr></td>
</tr>
<tr>
<td></td>
<td align=center colspan=2 valign=top><b><?= WEBPAGE::$gt['LN.RPT.income']; ?></b><br><small><b># <?= $loan->data[loan_code]; ?></b></small><br><br></td>
<td align=right><br><b><?= $date; ?></b></td>
</tr>
<tr>
<td colspan=4>
  <br>
  <?= sprintf(WEBPAGE::$gt[receipt],sprintf('<b>$ %s</b>',$pmt),sprintf('<b>%s</b>',$client)); ?>
  <br>
  <?= WEBPAGE::$gt[details]; ?>:
</td>
</tr>

<tr>
<td></td>
<td></td>
<td><?= WEBPAGE::$gt['tblPayments.principal']; ?>:</td>
<td align=right><b>$<?= number_format($ka,2); ?></b></td>
</tr>
<tr>
<td></td>
<td></td>
<td><?= WEBPAGE::$gt['tblPayments.interes']; ?>:</td>
<td align=right> <b>$<?= number_format($ra,2); ?></b></td>
</tr>
<tr>
<td></td>
<td></td>
<td><?= WEBPAGE::$gt['tblPayments.delay']; ?>:</td>
<td align=right><b>$<?= number_format($delay,2); ?></b></td>
</tr>

<tr>
<td><?= WEBPAGE::$gt['tblPayments.pending_principal']; ?>:</td>
<td><b>$<?= number_format($pk,2); ?></b></td>
<td><?= WEBPAGE::$gt['total']; ?>:</td>
<td align=right><b>$<?= number_format($pmt,2); ?></b></td>
</tr>
<tr>
<td colspan=4><hr></td>
</tr>
<tr>
<td></td>
<td colspan=2 align=center><br><br><hr><b><?= $user ?></b></td>
<td></td>
</tr>
</table>
<br><br>