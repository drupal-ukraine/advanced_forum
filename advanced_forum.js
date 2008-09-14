function jq_collapse (container_id) {
	$(document).ready(function(){
	$('#forum-list-' + container_id).click(function() {
  	$('.in-container-' + container_id).toggle()
  	if($.cookie('cookie_container-' + container_id) !== 'collapsed') {
   	 $.cookie('cookie_container-' + container_id, 'collapsed');
		}
		else {
			$.cookie('cookie_container-' + container_id, 'expanded');
		}
	});
  return false;
	});
}