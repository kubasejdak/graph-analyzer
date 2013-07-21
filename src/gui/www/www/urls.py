from django.conf.urls import patterns, include, url
from tasks.views import render_tasksManage, render_listAnalyzeTasks, render_createAnalyzeTask, render_listGroupTasks, render_createGroupTask, render_createExportTask
from analyze.views import render_listSamples, render_showSample, render_compareSamples, render_showAnalyzeTask
from group.views import render_showGroup, render_showGroupTask
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
    url(r'^jsejdak/create/task/analyze/$', render_createAnalyzeTask),
    url(r'^jsejdak/create/task/group/$', render_createGroupTask),
    url(r'^jsejdak/create/task/export/$', render_createExportTask),
    url(r'^jsejdak/manage/tasks/$', render_tasksManage),
    url(r'^jsejdak/list/samples/$', render_listSamples),
    url(r'^jsejdak/list/tasks/analyze/$', render_listAnalyzeTasks),
    url(r'^jsejdak/list/tasks/group/$', render_listGroupTasks),
    url(r'^jsejdak/show/tasks/analyze/$', render_showAnalyzeTask),
    url(r'^jsejdak/show/tasks/group/$', render_showGroupTask),
    url(r'^jsejdak/show/sample/$', render_showSample),
    url(r'^jsejdak/show/group/$', render_showGroup),
    url(r'^jsejdak/options/$', render_options),
    url(r'^jsejdak/show/group/$', render_showGroup),
    url(r'^jsejdak/feedback/$', render_feedback),
    url(r'^jsejdak/compare/samples/$', render_compareSamples),
)
