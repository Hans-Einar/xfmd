#include "application/adapters/SharedTextMetrics.h"
#include "renderer/MarkdownRenderer.h"
#include "renderer/PageBreaker.h"
#include "interpreter/CmarkInterpreter.h"
#include "support/TestSupport.h"
using namespace xfmd;
void run() {
  SharedTextMetrics metrics;CmarkInterpreter parser;MarkdownRenderer renderer;
  LayoutRequest request;request.profile.mode=LayoutMode::Paged;
  std::string source="# Page test\n\n";
  for(int i=0;i<100;++i)source+="Paragraph "+std::to_string(i)+" with æøå and **bold** content.\n\n";
  source+="```\n";
  for(int i=0;i<250;++i)source+="é";
  source+="\n```\n";
  auto model=parser.parse({{1,1},source,{},false});
  auto frame=renderer.layout(*model,request,metrics);
  CHECK(frame->pages.slices.size()>2);
  CHECK(frame->width==request.profile.paper.width);
  CHECK(frame->height==frame->pages.slices.size()*request.profile.paper.height);
  for(const auto& run:frame->runs) {
    auto y=std::fmod(run.bounds.y,request.profile.paper.height);
    CHECK(y>=request.profile.paper.margin-.01);
    CHECK(y+run.bounds.height<=request.profile.paper.height-request.profile.paper.margin+.01);
    CHECK(run.bounds.x+run.bounds.width<=request.profile.paper.width-request.profile.paper.margin+.01);
    if(run.font.mono && !run.text.empty())CHECK((static_cast<unsigned char>(run.text.front())&0xc0)!=0x80);
  }
  auto wider=request;wider.width=1200;
  auto same=renderer.layout(*model,wider,metrics);
  CHECK(same->pages.slices.size()==frame->pages.slices.size());
  CHECK(same->runs.size()==frame->runs.size());
  for(std::size_t i=0;i<frame->runs.size();++i)CHECK(same->runs[i].bounds.y==frame->runs[i].bounds.y);
  auto empty=renderer.layout(*parser.parse({{2,0},"",{},false}),request,metrics);
  CHECK(empty->pages.slices.size()==1);
  FlowLayout lines;lines.lines={{0,30,0},{30,20,2},{50,20,0},{70,20,0}};
  PaperSpec paper{100,100,10};
  auto split=PageBreaker::breakLines(lines,paper);
  CHECK(split.size()==2 && split[1].flowTop==30); // Heading and next two lines.
  request.cancelled=[] {return true;};
  bool cancelled=false;try {renderer.layout(*model,request,metrics);}catch(const Error&){cancelled=true;}
  CHECK(cancelled);
  paper.margin=std::numeric_limits<double>::quiet_NaN();
  bool invalid=false;try {PageBreaker::breakLines(lines,paper);}catch(const Error&){invalid=true;}
  CHECK(invalid);
}
TEST_MAIN(run)
