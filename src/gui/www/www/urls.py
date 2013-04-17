from django.conf.urls import patterns, include, url
from tasks.views import render_tasksManage
from analyze.views import render_analyzeTask
from group.views import render_groupTask
from browse.views import render_browseSamples, render_browseGroups, render_compareSamples, render_showSample, render_showGroup
from options.views import render_options
from home.views import render_home
from feedback.views import render_feedback

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'www.views.home', name='home'),
    # url(r'^www/', include('www.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Admin:
    url(r'^jsejdak/admin/', include(admin.site.urls)),
    
    # Main pages:
    url(r'^jsejdak/home/$', render_home),
    url(r'^jsejdak/tasks/manage/$', render_tasksManage),
    url(r'^jsejdak/tasks/analyze/$', render_analyzeTask),
    url(r'^jsejdak/tasks/group/$', render_groupTask),
    url(r'^jsejdak/browse_samples/$', render_browseSamples),
    url(r'^jsejdak/browse_groups/$', render_browseGroups),
    url(r'^jsejdak/options/$', render_options),
    url(r'^jsejdak/feedback/$', render_feedback),
    url(r'^jsejdak/compare/', render_compareSamples),
    url(r'^jsejdak/sample/', render_showSample),
    url(r'^jsejdak/group/', render_showGroup),
)
