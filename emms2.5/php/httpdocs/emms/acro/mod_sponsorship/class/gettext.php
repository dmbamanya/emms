<?php
class GETTEXT extends WEBPAGE
{

  /* GETTEXT parameters */
  var $data = array('tblBusinessTypes'  => array(0 => 'type'),
                    'tblCurrencys'      => array(0 => 'currency'),
                    'tblFunds'          => array(0 => 'fund'),
                    'tblLoanTypes'      => array(0 => 'description'),
                    'tblPrograms'       => array(0 => 'program'),
                    'tblSurveyItems'    => array(0 => 'question',       1 => 'answer_txt'),
                    'tblSurveys'        => array(0 => 'name',           1 => 'description'));

  function GETTEXT()  //class constructor
  {
  }

  static function START()
  {
    $start = new GETTEXT();
  }

  function add($id='')
  {
    if ($id) {

      WEBPAGE::$dbh->query(sprintf("insert into tblGetText (id) values ('%s') on duplicate key update id = id",$id));

    } else {
        
      foreach($this->data as $tbl => $cols) {
        foreach ($cols as $key => $col ) {
          WEBPAGE::$dbh->query(sprintf("insert into tblGetText (tblGetText.id) select %s.%s from %s on duplicate key update tblGetText.id = tblGetText.id",$tbl,$col,$tbl));
          }
       }

    }
  }

  function _($id)
  {
    $text = current(current(WEBPAGE::$dbh->getAll(sprintf("select %s from tblGetText where id = '%s'",WEBPAGE::$lang,$id))));
    return $text ? $text : $id;
  }

}
?>
