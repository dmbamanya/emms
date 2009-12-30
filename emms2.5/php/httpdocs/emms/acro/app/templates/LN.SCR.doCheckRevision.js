{javascript}
function refreshParent()
{
  window.opener.document.searchForm.submit();
  window.close();
}